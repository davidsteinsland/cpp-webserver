#!/bin/bash
# solhttpd daemon

DAEMON_PATH="$(dirname ${BASH_SOURCE[0]})/"

DAEMON=solhttpd
DAEMONOPTS=""

NAME=solhttpd
DESC="Sollandet HTTP Server"
PIDFILE=$DAEMON_PATH$NAME.pid
SCRIPTNAME=$DAEMON_PATH$NAME

case "$1" in
start)
	printf "%-50s" "Starting $NAME..."
	cd $DAEMON_PATH
	PID=`$DAEMON $DAEMONOPTS > /dev/null 2>&1 & echo $!`
	#echo "Saving PID" $PID " to " $PIDFILE
	if [ -z $PID ]; then
		printf "%s\n" "Fail"
	else
		echo $PID > $PIDFILE
		printf "%s\n" "Ok"
	fi
;;
status)
	printf "%-50s" "Checking $NAME..."
	if [ -f $PIDFILE ]; then
		PID=`cat $PIDFILE`
		if [ -z "`ps axf | grep ${PID} | grep -v grep`" ]; then
			printf "%s\n" "Process dead but pidfile exists"
		else
			echo "Running"
		fi
	else
		printf "%s\n" "Service not running"
	fi
;;
stop)
	printf "%-50s" "Stopping $NAME"
		
	if [ -f $PIDFILE ]; then
		PID=`cat $PIDFILE`
		cd $DAEMON_PATH

		kill -HUP $PID
		printf "%s\n" "Ok"
		rm -f $PIDFILE
	else
		printf "%s\n" "pidfile not found"
	fi
;;

restart)
	$0 stop
	$0 start
;;

*)
        echo "Usage: $0 {status|start|stop|restart}"
        exit 1
esac