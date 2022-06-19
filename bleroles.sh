#!/bin/bash

DIR="/home/blefnk/bleroles"
DESC="Bleroles"
EXEC="samp03svr"
SCREENNAME="bleroles"

function startServer {
	if [ -d $DIR ]; then
		cd $DIR
		if [ -f $EXEC ]; then
			screen -dmS $SCREENNAME ./$EXEC
			echo "$DESC started!"
		else
			echo "Error: OMP executable ($EXEC) not found!"
		fi
	else
		echo "Error: OMP directory ($DIR) not found!"
	fi
}

function stopServer {
	CHECK=`ps u -C $EXEC | grep -vc USER`
	if [ $CHECK -eq 0 ]; then
		echo "$DESC is currently not running."
	else
		killall $EXEC
		echo "$DESC stopped!"
	fi
}

function serverStatus {
	CHECK=`ps u -C $EXEC | grep -vc USER`
	if [ $CHECK -eq 0 ]; then
		echo "$DESC is currently not running."
	else
		echo "$DESC is running."
	fi
}

case "$1" in
	start)
		startServer
		;;

	stop)
		stopServer
		;;

	restart)
		stopServer
		sleep 1
		startServer
		;;

	status)
		serverStatus
		;;


	*)
		echo "Usage: $0 {start|stop|restart|status}"
		exit 1
		;;

esac
exit
