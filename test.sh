#!/bin/bash

#
# -------------- script variables --------------
#

# settable variables
files='images/foto.pgm'
streamlen='100'
max_nw=16
executions=1
threshold=50
delay=0

# helper variables
gnuplot_file=gnuplot_commands
graph_dir=graphs
graphs_produced=""

#
# -------------- script functions --------------
#

# Print script usage
usage() {
	echo "usage:"
	echo "  ${0##*/} [-?] [-f <file-list>] [-s <streamlen-list>] [-w <max-nw>] [-e <executions>] [-t <threshold>] [-d <delay>]"
	echo
	echo "  -?      print this message"
	echo "  -f <file-list>"
	echo "          space separated list of image files used to perform tests."
	echo "          Defaults to \"images/foto.pgm\""
	echo "  -s <streamlen-list>"
	echo "          space separated list of (simulated) stream length used to perform tests."
	echo "          Defaults to \"100\""
	echo "  -w <max-nw>"
	echo "          maximum number of worker used. The parallel code is run using workers in the range from 1 up to this value."
	echo "          Defaults to 16"
	echo "  -e <executions>"
	echo "          specify how many times the sequential and parallel code must be executed to get the average service time."
	echo "          Defaults to 1"
	echo "  -t <threshold>"
	echo "          specify the threshold used to perform all tests. This value will always be the same in any execution."
	echo "          Defaults to 50"
	echo "  -d <delay>"
	echo "          time to wait between two consecutive executions."
	echo "          Defaults to 0"
	echo
}

# Check for script options and assign the 
# provided values to the variables, overriding
# the default values.
#
# Parameters:
#	$1 list of script arguments (usually "$@")
check_parameters() {
	# Check if the first argument is a positive integer
	#
	# Parameters:
	#	$1 the value to check againts
	#	$2 name of the option
	is_number() {
		local re='^[0-9]+$'
		if ! [[ $1 =~ $re ]] ; then
			echo "error: -$2 expects positive integer" >&2
			exit 1
		fi
	}

	# Check if the first argument is a positive decimal
	#
	# Parameters:
	#	$1 the value to check againts
	#	$2 name of the option
	is_decimal() {
		local re='^[0-9]+(\.[0-9]+)*$'
		if ! [[ $1 =~ $re ]] ; then
			echo "error: -$2 expects positive number (integer or decimal)" >&2
			exit 1
		fi
	}

	OPTIND=1
	while getopts ":f:s:w:e:t:d:?h" opt
	do
		case $opt in
		f)
			files=$OPTARG
			;;
		s)
			is_number $OPTARG $opt
			streamlen=$OPTARG
			;;
		w)
			is_number $OPTARG $opt
			max_nw=$OPTARG
			;;
		e)
			is_number $OPTARG $opt
			executions=$OPTARG
			;;
		t)
			is_number $OPTARG $opt
			threshold=$OPTARG
			;;
		d)
			is_decimal $OPTARG $opt
			delay=$OPTARG
			;;
		\?|:)
			usage
			exit 1
			;;
		esac
	done

	shift $((OPTIND-1))
	[ "$1" = "--" ] && shift
}

# Execute the specified program $executions times
# using the current image and streamlen, with the 
# fixed global threshold. The average time of
# execution is stored in a gobal variable with the
# specified name.
#
# Parameters:
#	$1 name of the binary to execute ('par' or 'seq')
#	$2 name of the variable that will contain the result
function average() {
	local binary=$1
	local _result_var=$2
	
	local temp=""
	
	for ((i=1;i<=executions;i++))
	do
		temp+=`./$binary $img $threshold $nstream $nw | head -n 1`
		temp+=" "
		sleep $delay
		echo -n .
	done
	
	eval $_result_var=`echo $temp | awk -f average.awk`
}

# Initalise the $output_file used to store the measured
# performances. This file will be used by gnuplot to 
# build the graphs.
#
# The columns index will be used to reference the source
# data in the 'create_graph_commands()' function.
#
# The file is overwritten if already exists.
create_data_file() {
	echo "# " > $output_file
	echo "# Tseq = $Tseq" >> $output_file
	echo "# " >> $output_file
	echo "#nw time scalab speedup" >> $output_file
	graphs_produced+="${output_file##*/} "
}

# Add one line of data to the data file.
# Data is written following the column order used when writing the
# comment haeders in 'create_data_file()' function.
add_data_line() {
	echo -n "$nw $avgTpar " >> $output_file	# nw and time columns
	echo -n `echo $Tpar1 $avgTpar | awk '{ print $1/$2 }'` >> $output_file	# scalability column
	echo -n " " >> $output_file
	echo `echo $Tseq $avgTpar | awk '{ print $1/$2 }'` >> $output_file	# speedup column
}

# Parameters:
#	$1 graph name
#	$2 column data number
#	$3 xlabel
#	$4 ylabel
#	$5 ideal formula
add_graph_commands() {
	local graph_name=$1
	local column_data=$2
	local x_label=$3
	local y_label=$4
	local ideal_formula=$5

	graphs_produced+="$graph_name"_"$graph_file "
	echo "set output \"$graph_dir/$graph_name\_$graph_file\"" >> $gnuplot_file
	echo "set title \"file: $img, nstream = $nstream, threshold = $threshold%\n$graph_name\"" >> $gnuplot_file
	echo "unset xlabel" >> $gnuplot_file
	if [ ! -z "$x_label" ]
	then
		echo "set xlabel \"$x_label\"" >> $gnuplot_file
	fi
	echo "unset ylabel" >> $gnuplot_file
	if [ ! -z "$y_label" ]
	then
		echo "set ylabel \"$y_label\"" >> $gnuplot_file
	fi
	
	echo "plot '$output_file' using 1:$column_data title \"achieved $graph_name\" with linespoints, \\" >> $gnuplot_file
	echo "$ideal_formula title \"ideal $graph_name\"" >> $gnuplot_file
}

# Fill the $gnuplot_file with the gnuplot commands needed
# to build the different graphs for the current execution
# set.
#
# The file is overwritten if already exists.
create_gnuplot_file() {
	echo "set terminal pbm color" > $gnuplot_file

	# service time graph
	add_graph_commands "completion_time" 2 "nw" "time (ms)" "$Tseq/x"
	
	# scalability graph
	add_graph_commands "scalability" 3 "nw" "" "x"
 
	# speedup graph
	add_graph_commands "speedup" 4 "nw" "" "x"
}

#
# -------------- computation --------------
#

check_parameters "$@"

if [[ ! -e "par" || ! -e "seq" ]]
then
	echo "binary not found, rebuilding..."
	echo 
	make clean
	make
	echo
fi

if (( $executions <= 3 ))
then
	echo "Warning: the average will include best and worst results"
	echo
fi

for img in $files
do
	echo "===> processing $img <==="
	echo "     threshold = $threshold%"
	
	for nstream in $streamlen
	do
		echo
		echo "-- streamlen = $nstream"

		echo -n "executing sequential code"

		average "seq" Tseq

		echo

		graph_file=${img##*/}.$nstream.pbm
		output_file=$graph_dir/'data'.${img##*/}.$nstream.txt

		create_gnuplot_file

		create_data_file

		echo "executing parallel code"
		nw=0
		for ((j=0;nw<max_nw;j++))
		#for nw in 1 `seq 2 2 $max_nw`
		do
			nw=$((2**$j))
			if (( nw>max_nw ))
			then
				nw=$max_nw
			fi

			echo -n "  using $nw workers"

			average par avgTpar

			echo

			# save the parallel execution time with 1 worker to compute scalability
			if ((nw == 1))
			then
				Tpar1=$avgTpar
			fi

			add_data_line
		done

		gnuplot $gnuplot_file > /dev/null
	done
	
	echo
done

rm -f $gnuplot_file
echo graphs produced: $graphs_produced
echo
