pragma solidity ^0.5.0;

contract mortal
{
    address payable owner;

    constructor()
        public
    {
        owner = msg.sender;
    }

    function kill()
        public
    {
        if (msg.sender == owner)
            selfdestruct(owner);
    }
}

contract MutualAidFund is mortal
{
    mapping(address payable => )
}

