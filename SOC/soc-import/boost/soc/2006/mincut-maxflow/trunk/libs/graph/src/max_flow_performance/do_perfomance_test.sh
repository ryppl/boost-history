#!/bin/sh

EXE_DIR=bin/gcc/release

KOLMO_EXE=$EXE_DIR/kolmogorov
BOOST_PUSH_EXE=$EXE_DIR/boost_push-relabel
BOOST_KOLMO_EXE=$EXE_DIR/boost_kolmo
H_PRF_EXE=external/h_prf/hi_pr
H_PRFW_EXE=external/h_prf/hi_prw
LEDA_EXE=$EXE_DIR/leda
LEDA__STATIC_EXE=$EXE_DIR/optimization-off/leda_static

#number of repeats to get nice timestats
NUMBER_OF_TEST_REPEATS=10

#which executables to consider
EXE_FILES="$KOLMO_EXE $BOOST_PUSH_EXE $BOOST_KOLMO_EXE $H_PRF_EXE $H_PRFW_EXE $LEDA_EXE $LEDA__STATIC_EXE"
#description line 
RESULT_DESCRIPTION="#filename vertices edges kolmogorov pushrelabel boost_kolmo hi_pr hi_prw leda leda_static"

###
# check if executables are there, if not, build them
if [ ! -e $KOLMO_EXE ];then
	bjam --v2
fi

if [ ! -e $H_PRF_EXE ];then
	cd external/h_prf
	make
	cd ../..
fi

case $1 in
"" ) echo "Usage: generate_test_cases filename(s) ";;

*)
	FILE_LOG=results/file.log
	echo $RESULT_DESCRIPTION > $FILE_LOG
	for i; do
		datafile="$i"
		if [ -e "$datafile" ]; then
        NUMBER_OF_NODES=`grep "p max" $datafile | cut -d' ' -f3`
        NUMBER_OF_EDGES=`grep "p max" $datafile | cut -d' ' -f4`
		  LOGFILE_LINE="$datafile $NUMBER_OF_NODES $NUMBER_OF_EDGES"
        LAST_FLOW=""
		  echo "Running tests on file $datafile"
        for exe in $EXE_FILES
        do
          echo "$exe ... "
          i=0
          time_sum=0
          while [ $i -lt $NUMBER_OF_TEST_REPEATS ]
          do
            echo "$exe < $datafile"
            RESULT=`$exe < "$datafile"`
            FLOW_VALUE=`echo $RESULT | cut -d' ' -f2`
            TIME_VALUE=`echo $RESULT | cut -d' ' -f4`
            if [ "$LAST_FLOW" = "" ] ; then
              LAST_FLOW=$FLOW_VALUE
            fi
            if [ "$LAST_FLOW" != "$FLOW_VALUE" ] ; then
              echo "--------------------------------------------------------------------------------------"
              echo "WARNING! Flow values don't match! Check results which implementation caused the error!"
              echo "--------------------------------------------------------------------------------------"
            fi
            echo "RUN $i; time / flow: $TIME_VALUE / $FLOW_VALUE"
            time_sum=`echo "scale=3; $time_sum + $TIME_VALUE" | bc`
            ((++i))
          done
          average_time=`echo "scale=3; $time_sum / $NUMBER_OF_TEST_REPEATS" | bc`
          echo "average time: $average_time [s]"
          echo "flow: $FLOW_VALUE"
          LOGFILE_LINE="$LOGFILE_LINE $average_time"
        done
        echo $LOGFILE_LINE >> $FILE_LOG
		else
		  echo "File not found $datafile"
		  exit 
		fi
	done
esac
exit
