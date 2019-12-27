
function myLoadContract(file, cn)
{
    loadScript(file + '.js');
    var contractAbi = compilerOutput.contracts[file + ':' + cn].abi;
    var contract = eth.contract(JSON.parse(contractAbi));
    var binCode = "0x" + compilerOutput.contracts[file + ':' + cn].bin;
    return { abi : contract, bin : binCode };
}

function myDeployContract(contract, args, account, _gas)
{
    account = account || eth.accounts[0];
    _gas = _gas || 1000000;
    var deployTxObject = { from: account, data: contract.bin, gas: _gas };
    args.push(deployTxObject);
    var contractInstance = contract.abi.new.apply(contract.abi, args);
    var contractAddress = eth.getTransactionReceipt(contractInstance.transactionHash).contractAddress;
    var instance = contract.abi.at(contractAddress);
    return instance;
}

function myMethodApply(method, args, account, _gas)
{
    account = account || eth.accounts[0];
    _gas = _gas || 1000000;
    args.push({ from: account, gas: _gas });
    return method.sendTransaction.apply(method, args);
}
