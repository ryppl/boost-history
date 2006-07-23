#!/bin/sh

EXE_DIR=bin/gcc/release

KOLMO_EXE=$EXE_DIR/kolmogorov
BOOST_PUSH_EXE=$EXE_DIR/boost_push-relabel
BOOST_KOLMO_EXE=$EXE_DIR/boost_kolmo
H_PRF_EXE=external/h_prf/hi_pr
H_PRFW_EXE=external/h_prf/hi_prw

DATADIR=data

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


run_ak_test(){
  AK_DATAFILES=`ls $DATADIR/*_ak_*.dat`
  AK_LOG=results/ak.log
  echo "Running AK tests"
  echo "#filename vertices edges kolmogorov pushrelabel boost_kolmo hi_pr hi_prw" > $AK_LOG
  for datafile in $AK_DATAFILES
  do
    echo "Running tests with $datafile" 
    
    NUMBER_OF_NODES=`grep "p max" $datafile | cut -d' ' -f3`
    NUMBER_OF_EDGES=`grep "p max" $datafile | cut -d' ' -f4`
    LOGFILE_LINE="$datafile $NUMBER_OF_NODES $NUMBER_OF_EDGES"
    
    echo "Kolmogorov..." 
    LOGFILE_LINE="$LOGFILE_LINE `$KOLMO_EXE < $datafile | grep time: |sed 's/time: //'` "
    echo "Push-Relabel..." 
    LOGFILE_LINE="$LOGFILE_LINE `$BOOST_PUSH_EXE < $datafile | grep time: |sed 's/time: //'` "
    echo "Boost::Kolmo..." 
    LOGFILE_LINE="$LOGFILE_LINE `$BOOST_KOLMO_EXE < $datafile | grep time: |sed 's/time: //'` "
    echo "hi_pr..." 
    LOGFILE_LINE="$LOGFILE_LINE `$H_PRF_EXE < $datafile | grep time: |sed 's/time: //'` "
    echo "hi_prw..." 
    LOGFILE_LINE="$LOGFILE_LINE `$H_PRFW_EXE < $datafile | grep time: |sed 's/time: //'` "
    echo $LOGFILE_LINE >> $AK_LOG
  done
}

run_ac_test(){
  AC_DATAFILES=`ls $DATADIR/*_ac_*.dat`
  AC_LOG=results/ac.log
  echo "Running AC tests"
  echo "#filename vertices edges kolmogorov pushrelabel boost_kolmo hi_pr hi_prw" > $AC_LOG
  for datafile in $AC_DATAFILES
  do
    echo "Running tests with $datafile" 
    NUMBER_OF_NODES=`grep "p max" $datafile | cut -d' ' -f3`
    NUMBER_OF_EDGES=`grep "p max" $datafile | cut -d' ' -f4`
    LOGFILE_LINE="$datafile $NUMBER_OF_NODES $NUMBER_OF_EDGES"
    echo "Kolmogorov..." 
    LOGFILE_LINE="$LOGFILE_LINE `$KOLMO_EXE < $datafile |tee grep time: |sed 's/time: //'` "
    echo "Push-Relabel..." 
    LOGFILE_LINE="$LOGFILE_LINE `$BOOST_PUSH_EXE < $datafile |tee grep time: |sed 's/time: //'` "
    echo "Boost::Kolmo..." 
    LOGFILE_LINE="$LOGFILE_LINE `$BOOST_KOLMO_EXE < $datafile |tee grep time: |sed 's/time: //'` "
    echo "hi_pr" 
    LOGFILE_LINE="$LOGFILE_LINE `$H_PRF_EXE < $datafile |tee grep time: |sed 's/time: //'` "
    echo "hi_prw" 
    LOGFILE_LINE="$LOGFILE_LINE `$H_PRFW_EXE < $datafile |tee grep time: |sed 's/time: //'` "
    echo $LOGFILE_LINE >> $AC_LOG
  done
}

case $1 in
"AC") run_ac_test ;;

"AK") run_ak_test ;;

"" ) echo "Usage: generate_test_cases {AC|AK|filename(s)} ";;

*)
	FILE_LOG=results/file.log
	echo "#filename vertices edges kolmogorov pushrelabel boost_kolmo hi_pr hi_prw" > $FILE_LOG
	for i; do
		datafile="$i"
		if [ -e "$datafile" ]; then
			NUMBER_OF_NODES=`grep "p max" $datafile | cut -d' ' -f3`
			NUMBER_OF_EDGES=`grep "p max" $datafile | cut -d' ' -f4`
			LOGFILE_LINE="$datafile $NUMBER_OF_NODES $NUMBER_OF_EDGES"
			echo "Running tests on file $datafile"
			echo "Kolmogorov:" 
			LOGFILE_LINE="$LOGFILE_LINE `$KOLMO_EXE < $datafile | grep time: |sed 's/time: //'` "
			echo "Push-Relabel:" 
			LOGFILE_LINE="$LOGFILE_LINE `$BOOST_PUSH_EXE < $datafile | grep time: |sed 's/time: //'` "
			echo "Boost::Kolmo:" 
			LOGFILE_LINE="$LOGFILE_LINE `$BOOST_KOLMO_EXE < $datafile | grep time: |sed 's/time: //'` "
			echo "hi_pr" 
			LOGFILE_LINE="$LOGFILE_LINE `$H_PRF_EXE < $datafile | grep time: |sed 's/time: //'` "
			echo "hi_prw" 
			LOGFILE_LINE="$LOGFILE_LINE `$H_PRFW_EXE < $datafile | grep time: |sed 's/time: //'` "
			echo $LOGFILE_LINE >> $FILE_LOG
		else
			echo "File not found $datafile"
			exit 
		fi
	done
esac
exit
