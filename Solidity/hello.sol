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

contract greeter is mortal
{
    string greeting;

    constructor(string memory _greeting)
        public
    {
        greeting = _greeting;
    }

    // Эта функция и отвечает за возвращение "Hello, world!"
    function greet()
        public
        view
        returns (string memory)
    {
        return greeting;
    }

    function mygreet()
        public
        view
        returns (string memory)
    {
        if (msg.sender == owner)
        {
            return greeting;
        }
        else
        {
            return "no u";
        }
    }
}
