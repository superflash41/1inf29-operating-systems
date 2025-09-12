gcc leeDirexFat.c -o leeDirexFat -lm
gcc getFirstClusters.c -o getFirstClusters -lm
gcc freeClusters.c -o freeClusters -lm
gcc chainClusters.c -o chainClusters -lm

echo "LABORATORIO 7 - SAYMON NICHO (20211866)"
echo ""
echo "PREGUNTA 1"
./leeDirexFat usb20242.img
echo ""
echo ""
echo "INFO ADICIONAL"
./getFirstClusters usb20242.img
echo ""
echo ""
echo "PREGUNTA 2"
./freeClusters usb20242.img 6
./freeClusters usb20242.img 8
./freeClusters usb20242.img 72
./freeClusters usb20242.img 114
./freeClusters usb20242.img 118
./freeClusters usb20242.img 100000
echo ""
echo ""
echo "PREGUNTA 3"
./chainClusters usb20242.img
