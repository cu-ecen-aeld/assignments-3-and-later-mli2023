#!/bin/bash
PIDFILE="/run/aesdsocket.pid"
CUR_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DAEMON="/usr/bin/aesdsocket";
DAEMON_USER=ampac

do_start() {        
    start-stop-daemon --start \
    -n aesdsocket \
    --chuid "$DAEMON_USER" \
    --exec "$DAEMON" -- -d
}

do_stop() {
    start-stop-daemon -K -n aesdsocket
}

case "$1" in 
    start)
        echo "start.."
            # start-stop-daemon --start \
            #                 --chuid "$DAEMON_USER" \
            #                 --make-pidfile \
            #                 --pidfile "$PIDFILE" \
            #                 --exec "$DAEMON" -- -d
        do_start
        ;;
    stop)
        echo "stop.."
        # the pid in the file is parent pid, not child pid, so we cannot kill the process in this way
        # start-stop-daemon --stop --signal TERM --pidfile "$PIDFILE"
        # rm -f "$PIDFILE"
        do_stop
        ;;
    restart)
        $0 stop
        $0 start
        ;;
    *)
        echo $1 "is not supported"
        ;;
esac

exit 0
