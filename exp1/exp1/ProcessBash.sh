./exp1_endless.exe > a.txt & # execute endless in background

sleep 2

ps -ef

echo ""
pstree -h

echo ""
vmstat

echo ""
strace ./exp1.exe

echo ""
ltrace ./exp1.exe

sleep 2 &

echo ""
jobs

endlessPid=$(pidof exp1_endless.exe)
echo $endlessPid
kill $endlessPid

sleep 1
echo ""
jobs