for parFile in $@
do
	if [ ! -e ${parFile} ]; then
		echo "Parameter file ${parFile} does not exist."
	else
		echo "----------------------------------------------------------------------------"
		echo "Processing ${parFile}"
		python3 scripts/gen_scripts.py ${parFile} -all
		python3 scripts/gen_fieldmap.py ${parFile}
	fi
done

