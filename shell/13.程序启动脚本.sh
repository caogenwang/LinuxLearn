#!bin/bash

GMOND=/user/sbin/gmond

./etc/rc.d/init.d/function

RETVAL=0

case "$1" in
    start)
        echo -n "Starting GANGLIA gmond: "
        [ -f $GMOND ] || exit 1
        #后台运行
        daemon $GMOND
        #将上一命令的退出状态码赋给变量RETVAL
        RETVAL=$?
        #命令执行成功则创建一个文件
        [ $RETVAL eq 0 ] && touch /var/lock/subsys/gmond
    ;;
    stop)
        echo -n "Shutting down GANGLIA gmond: "
        killproc gmond
        RETVAL=$?
        echo $RETVAL
        [ $RETVAL eq 0 ] && rm -f /var/lock/subsys/gmond
    ;;
    restart|reload)
        $0 stop
        $0 start
        RETVAL=$?
        echo $RETVAL
    ;;
    status)
        status gmond
        RETVAL=$?
        echo $RETVAL
    ;;
    *)
        echo -n "Usage:$0 {start|stop|restart|reload|status}"
        exit 1
    ;;
esac
exit $RETVAL