#!/usr/bin/env bash

EXPERS="A,30500 B,22600 C,37000"
METRICS="caf arr"
GRMETS="caf-A,30500 caf-B,22600 caf-C,37000 caf-X,33240 caf-Y,84500 arr-A,30500 arr-B,22600 arr-C,37000 arr-X,33240 arr-Y,84500 "
GOLD_SEED=378371
GOLD_RUNS=1
EXAMPLE_SEEDS="646 289 280574"
EXAMPLE_N=3
EXAMPLE_RUNS=1
GRADE_SEED=226876
GRADE_RUNS=1
rubric_line_item()
{
	local i=`case $1:$2 in
		*,arr ) echo 1000 ;;
		A,*:caf ) echo 0 ;;
		B,*:caf ) echo 1 ;;
		C,*:caf ) echo 2 ;;
		X,*:caf ) echo 3 ;;
		F,*:caf ) echo 4 ;;
		* ) echo 999 ;;  # raise fault on misconfig
	esac`
	expr 3 + $i
}

unpack_experiment()
{
	echo "${1}" |sed -E -e 's/^([^,]+),([^,]+)$/pwdefn=\1;Q=\2;/'
	return 0
}

checkwrap()
{
	local tK=${1}
	if ! awk -v "tK=${tK}" -f "${graderloc}/checkwrap.awk" ; then
		exit 1
	fi
}

missingdata=0
experiment()
{
	local theSIM="${1}"
	local exper="${2}"
	local RUNS="${3}"
	local SEED="${4}"		
	local missingdata=0
	local residfmt="__residual-%s-%s-%d.%s"
	local residrand
	local residoutput

	local pwdefn
	local Q
	eval `unpack_experiment ${2}`

	local tK=`tail -n 1 "${graderloc}/arr${pwdefn}.dat" | awk '{print $1}'`
	local bins=${GRADERMODEBINS:-60}

	local RQ=`expr $RUNS \* $Q`
	local data=__aitk-${exper}-$SEED-$RUNS.dat

    for ((i=0; i<${RUNS}; i++)); do 
        test $(( $i % 50 )) -eq 0 && echo -n >&2 $i " "
        residrand=`printf ${5:-${residfmt}} $exper random $i dat 2>/dev/null`
        residoutput=`printf ${5:-${residfmt}} $exper output $i log 2>/dev/null`
        test_run -Q "${theSIM}" <(Random $(( SEED + i )) | tee "${residrand}" )\
				"${graderloc}/arr${pwdefn}.dat" ${Q} \
		    | tee "${residoutput}" \
			| "${SIMGRADING}/output-pipe" | "${SIMGRADING}/paddle" 2 \
			| run_checknumeric s6+ s6+ \
			| checkwrap ${tK}
    done >${data}
	echo ${RUNS} >&2

    test `ncdat ${data}` -ne ${RQ} && missingdata=1   

	for m in caf ; do 
		for x in _${m}-${exper}-${SEED}-${RUNS}.dat ; do
			lineswithdata ${data} | sort -R | head -n 2000 | sort -g |${SIMGRADING}/ecdf >${x}
		done
	done

	for m in arr ; do 
		for x in _${m}-${exper}-${SEED}-${RUNS}.dat ; do
			lineswithdata ${data} |\
			"${SIMGRADING}/bestbins" -q ${bins},0,${tK} -Q -O - ${data} > ${x}
		done
	done
	return ${missingdata}
}


###
# SIMS and usage should be defined before sourcing sim-lib.sh
###
SIMS=SIM
usage()
{
	cat <<EoT
ASSIGNMENT SPECIFIC OPTIONS 

  $ /assignment/specific/path/grader.sh . [RUNS [SEED]]

Where

  . is the SIM location (required first parameter)

  RUNS is the number of RUNS to use for SIM execution and plot generation

  SEED is a positive integer for a SEED to use for random file inputs

To retain residual data files to (maybe) assist in debugging, export

  $ export GRADER_SAVE_RESIDUALS=${SIMS}

EoT
}

# if SIMGRADING is unknown
test -z "${SIMGRADING}" -a -r ~khellman/SIMGRADING/sim-lib.sh && ~khellman/SIMGRADING/setup.sh ~khellman/SIMGRADING
if ! test -r "${SIMGRADING}/sim-lib.sh" ; then
	cat >&2 <<EoT
ERROR:  SIMGRADING is not in your environment or SIMGRADING/sim-lib.sh cannot
be found.  Have you followed the grader tarball setup.sh instructions on the
assignment's Wiki page?
EoT
	exit 1
fi

test -n "${SIMGRADING}" && source "${SIMGRADING}/sim-lib.sh"
source "${GRADERDIR}/grader-exec.sh"

set -e

RUNS=${1:-${GRADE_RUNS}}
SEED=${2:-${GRADE_SEED}}


# SIM args 
tallydir=`create_tally_dir 1 TYPE=auto MAX=5 "Trace file used correctly"`
cat <<EoT |grader_msg 
Running SIM with non-existent and truncated trace file.  Missing trace files
should not emit OUTPUT, truncated trace files may.  Both must exit non-zero.
EoT
tally_nonexist_tracefile "${tallydir}/missing" "${simloc}/SIM" MISSINGRANDOM "${graderloc}/arrA.dat" 8 
tally_truncated_tracefile_with_output "${tallydir}/trunc" "${simloc}/SIM" TRUNCRANDOM_100 "${graderloc}/arrA.dat" 800 

tallydir=`create_tally_dir 2 TYPE=auto MAX=5 "Missing or empty data file handled correctly"`
cat <<EoT |grader_msg 
Running SIM with non-existent or empty data file.  These runs should exit with
non-zero status and without OUTPUT lines...
EoT
tally_missing_datafile "${tallydir}/missing" "${simloc}/SIM" <("${SIMGRADING}/Random") MISSINGDATAFILE  8 
tally_empty_datafile "${tallydir}/empty" "${simloc}/SIM" <("${SIMGRADING}/Random") EMPTYDATAFILE  8 



# create tally descriptions for EXPERS x METRIC rubrics
r=0
for ex in ${EXPERS}; do
	x=${ex%,*}
	create_tally_dir `expr 4 + $r` TYPE=man MAX=10 "Arrival times for arr${x}.dat accurate"
	r=`expr $r + 1`
done >/dev/null

grader_msg <<EoT
Running several SIM experiments with varying parameters.  This may take 
some time ...
If the test exits with a "checknumeric" error, then one of your OUTPUT values
is improperly formatted (or missing data has confused the format analysis).
EoT
set -e
missingdata=0
for EX in ${EXPERS} ; do
	eval `unpack_experiment ${EX}`
	grader_msg <<EoT
Running pwdefn=${pwdefn} Q=${Q}
EoT
	experiment "${simloc}/SIM" ${EX} ${RUNS} ${SEED}
	missingdata=$?

    for grm in ${GRMETS} ; do 
		x=${grm%-*}
		ex=${grm#*-}
		case "${ex}" in
			"${EX}" ) 
			    grader_msg <<EoT
... generating comparison plots for ${x} ${EX} ...
EoT
				gplotpdf ${x} ${EX} ${RUNS} ${SEED} `rubric_line_item $EX $x` 0
				set +x
				;;
		esac
    done

	grader_cleanup_experiment ${missingdata} "${METRICS}" "${EX}" "${RUNS}" "${SEED}"

    grader_save_residuals ${EX}

done

grader_signoff ${RUNS} ${SEED}

