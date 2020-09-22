#include "PrizeList.h"

void PrizeList::add(Sprite* sprite, Label* label, const int& probability)
{
	sprites.push_back(sprite);
	labels.push_back(label);
	probabilities.push_back(probability);
}

//assumes probabilities add up to 100. If not, will return first prize in case of exception
int PrizeList::winPrize()
{
	//srand(time(0));
	int randomNum = rand() % 100 + 1;
	
	for (int i = 0, pSum = 0; i < probabilities.size(); i++)
	{
		pSum += probabilities[i];
		if (randomNum < pSum)
		{
			return i;
		}
	}

	return 0;
}

void PrizeList::testWinRate(int attempts)
{
	int results[8] = { 0 };

	for (int i = 0; i < attempts; i++)
	{
		results[winPrize()]++;
	}

	std::ofstream myfile;
	myfile.open("WinRateResults.txt");
	myfile << "Prize\t" << "Times won\t" << "Overall %\n";
	for (int i = 0; i < 8; i++)
	{
		myfile << i + 1 << ".\t" << results[i] << "\t\t" << 100 * results[i] / attempts << "%\n";
	}
	myfile.close();
}