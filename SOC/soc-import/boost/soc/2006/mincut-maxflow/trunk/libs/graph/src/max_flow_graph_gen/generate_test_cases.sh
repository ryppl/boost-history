#!/bin/bash

#path to binaries
BINDIR=bin/gcc/release

GEN_AC=$BINDIR/ac
AC_NODES="1024 1448 2048 2435 2896 3444 4096 4870 5792"
AC_SEED=`date +%s`

GEN_AK=$BINDIR/ak
AK_i="01000 02000 03000 04000 05000 06000 07000 08000 09000 10000 11000 12000 13000 14000 15000 16000 17000 18000 19000 20000 21000 22000 23000 24000 25000"

GEN_GENRMF=$BINDIR/genrmf
GENRMF_FRAMESIZE="32 x 64 "
GENRMF_DEPTH="2 x 1"
GENRMF_NODES="2048 2896 4096 5792 8192 11585 16394 23170 32768 46340 65536"
GENRMF_CAP_RANGE1=1
GENRMF_CAP_RANGE2=100

GEN_WASHINGTON=$BINDIR/washington

generate_ac_graphs ()
{
	if [ ! -e $GEN_AC ] 
	then
		echo "$GEN_AC doesn't exist"
	else
		echo "Generating ac graphs."
		for num_nodes in $AC_NODES
		do
			COMMAND="$GEN_AC $num_nodes $AC_SEED"
			echo "executing: $COMMAND. Saving to max_flow_ac_$num_nodes.dat"; 
			$COMMAND > max_flow_ac_$num_nodes.dat.tmp
			if [ ! $?  ];then
				echo "something went wrong. exiting."
				exit 1
			else
			# we remove one line from the AC-output, which reads s SEED_VALUE
			sed  "s/s $AC_SEED/c seed: $AC_SEED/1" max_flow_ac_$num_nodes.dat.tmp > max_flow_ac_$num_nodes.dat
			rm max_flow_ac_$num_nodes.dat.tmp
			fi
		done
		echo "Done."
	fi
}


generate_ak_graphs() 
{
	if [ ! -e $GEN_AK ] 
	then
		echo "$GEN_AK doesn't exist"
	else
		echo "Generating ak graphs."
		for aki in $AK_i
		do
		COMMAND="$GEN_AK $aki"
		echo "executing: $COMMAND. Saving to max_flow_ak_$aki.dat"; 
		$COMMAND > max_flow_ak_$aki.dat
		if [ ! $?  ];then
			echo "something went wrong. exiting."
			exit 1
		fi
		done
		echo "Done."
	fi
}

# if [ ! -e $GEN_GENRMF ] 
# then
# 	echo "$GEN_GENRMF doesn't exist"
# else
# 	echo "Generating rmf graphs."
# 	for num_nodes in $GENRMF_NODES
# 	do
# 	COMMAND="$GEN_GENRMF -max_flow_ak_$num_nodes.dat -a $num_nodes"
# 	echo "executing: $COMMAND. Saving to max_flow_rmf_$num_nodes.dat"; 
# 	$COMMAND 
# 	if [ ! $?  ];then
# 		echo "something went wrong. exiting."
# 		exit 1
# 	fi
# 	done
# 	echo "Done."
# fi




case $1 in
"AC") generate_ac_graphs ;;

"AK") generate_ak_graphs ;;

*) echo "Usage: generate_test_cases {AC|AK|GENRMF} ";;

esac
exit

