echo "kill " $* >> /tmp/executa.log
sleep $1
echo "kill -9 " $2 >> /tmp/executa.log
kill -9 $2
