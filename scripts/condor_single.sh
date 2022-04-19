subfile="condor_`date +%F-%H-%M-%S`"
mkdir -p ${subfile} 

cat > ${subfile}/$subfile.sub << EOF
Executable   = $1
Arguments    = $2 ${3} $4 $5 $6 $7 $8 $9
InitialDir   = `pwd`
Log          = ${subfile}/${1}.log
Error        = ${subfile}/${1}.err
Output       = ${subfile}/${1}.out
#Input        = WHEN_YOU_HAVE_INPUT_FILES
Universe     = vanilla
GetEnv       = True
Request_memory = 2G
Request_cpus = 1
Notification = Always
Requirements = machine =!= "kunpl07"
Notify_user  = simpson9794@korea.ac.kr
Queue
EOF

condor_submit ${subfile}/$subfile.sub
rm ${subfile}/$subfile.sub
