#include "pch.h"
#include <iostream>  // Für std::cout, std::cin, std::endl
#include <iomanip> 


typedef long long int64;
typedef int64 CAmount;
const CAmount COIN = 100000000;

struct CBlockHeader {
	// Dummy structure for this example
};

namespace Consensus {
	struct Params {
		int nSubsidyHalvingInterval;
		int nBudgetPaymentsStartBlock;
	};
}

CAmount GetBlockSubsidy(int nHeight, CBlockHeader pblock, const Consensus::Params& consensusParams, bool fSuperblockPartOnly) {
	CAmount nSubsidy = 12.5 * COIN;

	if (nHeight == 1) {
		nSubsidy = 16287337.5 * COIN;
		return nSubsidy;
	}

	int fork1 = 10000;
	int block_reward_change_f4 = 4;
	int BTXFullblock = (42987 + fork1) * 4;

	if (nHeight <= fork1) {
		return nSubsidy;
	}
	else {
		int nSubsidyHalvingInterval2 = 840000;
		nSubsidy /= block_reward_change_f4;
		int halvings = (nHeight + BTXFullblock) / nSubsidyHalvingInterval2;
		if (halvings >= 256) return 0;
		nSubsidy >>= halvings;
	}

	// We don't use that
	//CAmount nSuperblockPart = (nHeight > consensusParams.nBudgetPaymentsStartBlock) ? nSubsidy / 10 : 0;
	CAmount nSuperblockPart = (nHeight > consensusParams.nBudgetPaymentsStartBlock) ? nSubsidy / 10 : 0;
	//return fSuperblockPartOnly ? nSuperblockPart : nSubsidy - nSuperblockPart;
	return nSubsidy;

}

int main() {
	Consensus::Params consensusParams;
	consensusParams.nSubsidyHalvingInterval = 210000;
	consensusParams.nBudgetPaymentsStartBlock = 1000000;

	while (true) {
		int startHeight, endHeight, interval;
		bool fSuperblockPartOnly;
		bool showHalvingBlocks;
		std::cout << "Bitcore BTX halving Calculator 1\n\n";
		std::cout << "Enter start block height: ";
		std::cin >> startHeight;

		std::cout << "Enter end block height: ";
		std::cin >> endHeight;

		std::cout << "Enter interval: ";
		std::cin >> interval;

		std::cout << "Superblock part only? (1 for yes, 0 for no): ";
		std::cin >> fSuperblockPartOnly;

		std::cout << "Show only halving blocks? (1 for yes, 0 for no): ";
		std::cin >> showHalvingBlocks;

		CBlockHeader pblock;  // Dummy header

		if (showHalvingBlocks) {
			for (int height = startHeight; height <= endHeight; height += interval) {
				CAmount subsidy = GetBlockSubsidy(height, pblock, consensusParams, fSuperblockPartOnly);
				std::cout << "Block height " << height << " subsidy: " << std::fixed << std::setprecision(8) << static_cast<double>(subsidy) / COIN << " COIN" << std::endl;
			}
		}
		else {
			for (int height = startHeight; height <= endHeight; height += interval) {
				CAmount subsidy = GetBlockSubsidy(height, pblock, consensusParams, fSuperblockPartOnly);
				std::cout << "Block height " << height << " subsidy: " << std::fixed << std::setprecision(8) << static_cast<double>(subsidy) / COIN << " COIN" << std::endl;
			}
		}

		char choice;
		std::cout << "Do you want to start again? (y/n): ";
		std::cin >> choice;
		if (choice != 'y' && choice != 'Y') {
			break;
		}
	}

	return 0;
}
