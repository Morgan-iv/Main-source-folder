pragma solidity >=0.4.10;

contract Storage 
{
    uint256 storedData;

    function set(uint256 data)
        public
    {
        storedData = data;
    }

    function get()
        public
        view
        returns (uint256)
    {
        return storedData;
    }
}
