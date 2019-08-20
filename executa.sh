date >> /tmp/executa.log
echo "executa " $* >> /tmp/executa.log
PATH=.:$PATH
IMPL=$1
INST=$2
TOUT=60
$IMPL $INST &
PID=$!
/cefet/cefet/killtout.sh $TOUT $PID >/dev/null 2>&1 &
