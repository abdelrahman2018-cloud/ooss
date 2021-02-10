
function validate_backup_params(){

if [ -z $1 ]; then		#check if the user didn't pass any arguments
       echo "Please pass 3 arguments to the program to run! ex. ./backup.sh original_file backup_file password"	
fi

if [ ! -d $1 ]; then	#if he passed arguments, then check them
		echo "The first directory doesn't exist!"
fi

if [ ! -d $2 ]; then
	echo "The second directory doesn't exist!"
fi

if [ ! -z $3 ]; then 
	echo "Please enter the password!"
fi

}
#------------------------------------------------------------------

function backup(){
	
D=$(date | sed s/\ /_/g | sed s/\:/_/g)		#create bash variable with date
mkdir $B/$D					#create directory with the name

for d in $1/*
do
	if [ -d $d ]; then 		#for directory
		tar -cvf $d-$D.tar $d	#tar 
		gzip $d-$D.tar		#compress
		rm $d-$D.tar		#rempve tar version
		gpg -c $d-$D.tar.gz		#encrypt
		rm -r $d				#remove directory
		rm $d-$D.tar.gz			#remove compressed file
		mv $d-$D.tar.gz.gpg $2/$D
	fi
	
	if [ -f $d ]; then		#if file
	tar -uvf./$2/$D/FILES-$D.tar $d	#tar and update all files
	rm $d				#remove file
	fi
	
done

gzip ./$2/$D/FILES-$D.tar	#compress the tar for files
rm ./$2/$D/FILES-$D.tar		#remove the tar 
gpg -c ./$2/$D/FILES-$D.tar.gz	#encrypt the tar
rm ./$2/$D/FILES-$D.tar.gz	#remove the zipped file

for d in $2/$D/*
do	tar -uvf ./$2/$D/FINAL-FILE.tar $d	#tar all directories
	rm $d
done

gzip ./$2/$D/FINAL-FILE.tar	#compress the big file
rm ./$2/$D/FINAL-FILE.tar	#remove the tar version
gpg -c ./$2/$D/FINAL-FILE.tar.gz	#encrypt the big file
rm ./$2/$D/FINAL-FILE.tar.gz		#remove the zipped file

}

#-------------------------------------------------------------------

function validate_restore_params(){

if [ -z $1 ]; then 
	echo "Please pass 3 parameters to the program! ex. ./restore.sh backup_file original_file password"
fi

if [ ! -d $1 ]; then
	echo "The first directory doesn't exist!"
fi

if [ ! -d $2 ]; then
	echo "The second directory doesn't exist!"
fi

if [ -z $3 ]; then
	echo "Please enter the password!"
fi

}

#------------------------------------------------------------------

function restore(){

gpg $1/FINAL-FILE.tar.gz.gpg	#decrypt the whole file
rm $1/FINAL-FILE.tar.gz.gpg

gunzip $1/FINAL-FILE.tar.gz	#decompress the whole file

tar -xvf $1/FINAL-FILE.tar	#untar the whole file
rm $1/FINAL-FILE.tar

for d in $1/*	#loop to decrypt
do
	gpg $d
	rm $d
done

for d in $1/*	#loop to unzip
do
	gunzip $d
done

for d in $1/*	#loop to un tar
do
	tar -xvf $d
	rm $d
done

}

