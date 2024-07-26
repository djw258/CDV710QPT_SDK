#!/bin/sh
/config/riu_r 20 3
/config/riu_w 20 3 0
MAC00_15=$(/config/riu_r 20 16 | sed -n '2p')
MAC16_31=$(/config/riu_r 20 17 | sed -n '2p')
MAC32_47=$(/config/riu_r 20 18 | sed -n '2p')
MAC_BYTE=${MAC00_15:3:1}

if test $MAC_BYTE == "F";then
   MAC_LOW_ETH0=${MAC00_15:2:1}"E"
   MAC_LOW_ETH1=${MAC00_15:2:1}"2"
elif test $MAC_BYTE == "D";then
   MAC_LOW_ETH0=${MAC00_15:2:1}"C"
   MAC_LOW_ETH1=${MAC00_15:2:1}"E"
elif test $MAC_BYTE == "B";then
   MAC_LOW_ETH0=${MAC00_15:2:1}"A"
   MAC_LOW_ETH1=${MAC00_15:2:1}"C"
elif test $MAC_BYTE == "9";then
   MAC_LOW_ETH0=${MAC00_15:2:1}"8"
   MAC_LOW_ETH1=${MAC00_15:2:1}"A"
elif test $MAC_BYTE == "7";then
   MAC_LOW_ETH0=${MAC00_15:2:1}"6"
   MAC_LOW_ETH1=${MAC00_15:2:1}"8"
elif test $MAC_BYTE == "5";then
   MAC_LOW_ETH0=${MAC00_15:2:1}"4"
   MAC_LOW_ETH1=${MAC00_15:2:1}"6"
elif test $MAC_BYTE == "3";then
   MAC_LOW_ETH0=${MAC00_15:2:1}"2"
   MAC_LOW_ETH1=${MAC00_15:2:1}"4"
else
   MAC_LOW_ETH0=${MAC00_15:2:2}
   MAC_ETH0_BYTE=${MAC00_15:2:1}
   if test ${MAC_ETH0_BYTE} == "0";then
	MAC_LOW_ETH1="E"
	echo ${MAC_LOW_ETH1}
   else
   	MAC_LOW_ETH1=$((MAC_ETH0_BYTE-1))
	echo ${MAC_LOW_ETH1}
   fi

   MAC_LOW_ETH1=${MAC_LOW_ETH1}${MAC00_15:3:1}
  
fi

ETH0_MAC="${MAC_LOW_ETH0}:${MAC00_15:4:2}:${MAC16_31:2:2}:${MAC16_31:4:2}:${MAC32_47:2:2}:${MAC32_47:4:2}"
echo "eth0 mac:"${ETH0_MAC}
echo ${ETH0_MAC} > /appconfigs/run/eth0_mac

ETH1_MAC="${MAC_LOW_ETH1}${MAC_LOW_ETH1:2:1}:${MAC00_15:4:2}:${MAC16_31:2:2}:${MAC16_31:4:2}:${MAC32_47:2:2}:${MAC32_47:4:2}"
echo "eth1 mac:"${ETH1_MAC}
echo ${ETH1_MAC} > /appconfigs/run/eth1_mac

