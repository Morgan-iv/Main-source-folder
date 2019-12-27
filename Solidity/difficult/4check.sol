
pragma solidity ^0.4.18;

import './abstract/BaseUnilotGame.sol';
import './UnilotTailEther.sol';

contract UnilotBonusTailEther is BaseUnilotGame {
    mapping (address => TicketLib.Ticket[]) public tickets;
    mapping (address => uint) _prize;

    uint16 numTickets;

    uint64 winnerIndex;

    function UnilotBonusTailEther(address calculatorContractAddress)
        public
        payable
    {
        state = State.ACTIVE;
        administrator = msg.sender;

        calculator = UnilotPrizeCalculator(calculatorContractAddress);

        GameStarted(0);
    }

    function importPlayers(address game, address[] players)
        public
        onlyAdministrator
    {
        UnilotTailEther _game = UnilotTailEther(game);

        for (uint8 i = 0; i < uint8(players.length); i++) {
            TicketLib.Ticket memory ticket;

            var(block_number, block_time, prize) = _game.getPlayerDetails(players[i]);

            if (prize > 0) {
                continue;
            }

            ticket.block_number = uint40(block_number);
            ticket.block_time = uint32(block_time);

            if ( tickets[players[i]].length == 0 ) {
                ticketIndex.push(players[i]);
            }

            tickets[players[i]].push(ticket);
            numTickets++;
        }
    }

    function getPlayerDetails(address player)
        public
        view
        inactiveGame
        returns (uint, uint, uint)
    {
        player;

        return (0, 0, 0);
    }

    function ()
        public
        payable
        onlyAdministrator
    {

    }

    function getPrizeAmount()
        public
        constant
        returns (uint result)
    {
        return this.balance;
    }

    function calcaultePrizes()
        public
        returns(uint[] memory result)
    {
        var(numWinners, numFixedAmountWinners) = getNumWinners();
        uint16 totalNumWinners = uint16( numWinners + numFixedAmountWinners );
        result = new uint[]( totalNumWinners );


        uint[50] memory prizes = calculator.calcaultePrizes(
            getPrizeAmount()/ticketIndex.length, ticketIndex.length);

        for (uint16 i = 0; i < totalNumWinners; i++) {
            result[i] = prizes[i];
        }

        return result;
    }

    function getWinners()
        public
        view
        finishedGame
        returns(address[] memory players, uint[] memory prizes)
    {
        var(numWinners, numFixedAmountWinners) = getNumWinners();
        uint totalNumWinners = numWinners + numFixedAmountWinners;

        players = new address[](totalNumWinners);
        prizes = new uint[](totalNumWinners);

        uint index;

        for (uint i = 0; i < totalNumWinners; i++) {
            if ( i > winnerIndex ) {
                index = ( ( players.length ) - ( i - winnerIndex ) );
            } else {
                index = ( winnerIndex - i );
            }

            players[i] = ticketIndex[index];
            prizes[i] = _prize[players[i]];
        }

        return (players, prizes);
    }

    function finish()
        public
        onlyAdministrator
        activeGame
    {
        uint64 max_votes;
        uint64[] memory num_votes = new uint64[](ticketIndex.length);

        for (uint i = 0; i < ticketIndex.length; i++) {
            for (uint8 j = 0; j < tickets[ticketIndex[i]].length; j++) {
                TicketLib.Ticket memory ticket = tickets[ticketIndex[i]][j];

                uint64 vote = uint64( ( ( ( ticket.block_number * ticket.block_time ) / numTickets ) + (((block.number/2) * now) / (numTickets/2)) + uint( ticketIndex[i]) ) % ticketIndex.length );

                num_votes[vote] += 1;

                if ( num_votes[vote] > max_votes ) {
                    max_votes = num_votes[vote];
                    winnerIndex = vote;
                }
            }
        }

        uint[] memory prizes = calcaultePrizes();

        uint lastId = winnerIndex;

        for ( i = 0; i < prizes.length; i++ ) {
            _prize[ticketIndex[lastId]] = prizes[i];
            ticketIndex[lastId].transfer(prizes[i]);

            if ( lastId <= 0 ) {
                lastId = ticketIndex.length;
            }

            lastId -= 1;
        }

        administrator.transfer(this.balance); //For case of misscalculation

        state = State.ENDED;

        GameFinished(ticketIndex[winnerIndex]);
    }

    function revoke()
        public
        onlyAdministrator
        activeGame
    {
        administrator.transfer(this.balance);

        state = State.REVOKED;
    }
}
