#!/bin/bash

gnome-terminal -e 'geth --dev --rpc --rpcaddr "0.0.0.0" --rpcapi "admin,debug,miner,shh,txpool,personal,eth,net,web3" console'
#sleep 5
#gnome-terminal -e 'ethereumwallet --rpc http://localhost:8545'
