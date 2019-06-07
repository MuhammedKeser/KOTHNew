#include "Map.h"

Map Map::m_map = Map(0,0,0,0);


void Map::ProceduralGeneration()
{
	int rowCount = Map::m_map.GetHeight();
	int colCount = Map::m_map.GetWidth();

	//Initialize the map
	for (int i = 0; i < rowCount; i++)
	{
		m_map.m_grid[i] = (int*)calloc(colCount, sizeof(int));
	}
	for (int i = 0; i < rowCount; i++)
	{
		m_map.m_spriteGrid[i] = (Sprite**)calloc(colCount, sizeof(Sprite*));
	}

	//Fill in the borders
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			if (i == 0 || i == rowCount - 1 || j == 0 || j == colCount - 1)
				m_map.m_grid[i][j] = 1;
		}
	}

	//Provide center obstacles
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			//The closer it is to the midpoint to the center, the higher the likelyhood
			if (rand() % (abs(abs((i - rowCount / 2)) - rowCount / 4) + 1) < 1
				&& rand() % (abs(abs((j - colCount / 2)) - colCount / 4) + 1) < 1)
				m_map.m_grid[i][j] = 1;

			//The closer it is to the center, the higher the likelyhood
			if (rand() % (abs(i - rowCount / 2) + 1) < 1
				&& rand() % (abs(j - colCount / 2) + 1) < 1)
				m_map.m_grid[i][j] = 1;
		}
	}

	//todo->check 4 adjacent cells around each cell. If there's not even a single 1-valued cell, make this cell (and 3 random cells 1 cell adjacent to it) equal to 1

	//Fill in with some random obstacles
	int roundCount = 7;
	for (int curRound = 0; curRound < roundCount; curRound++)
	{
		for (int i = 0; i < rowCount; i++)
		{
			for (int j = 0; j < colCount; j++)
			{
				int neighborCount = 0;

				for (int neighborRow = -2; neighborRow <= 2; neighborRow++)
				{
					for (int neighborCol = -2; neighborCol <= 2; neighborCol++)
					{
						if (i + neighborRow < 0 || i + neighborRow >= rowCount
							|| j + neighborCol < 0 || j + neighborCol >= colCount)
							continue;

						if (neighborRow == 0 && neighborCol == 0)
							continue;

						if (m_map.m_grid[i + neighborRow][j + neighborCol] == 1)
							neighborCount++;

					}
				}

				if (neighborCount > 8 && rand() % 100 > 75)
					m_map.m_grid[i][j] = 1;
			}
		}
	}

	roundCount = 2;
	for (int curRound = 0; curRound < roundCount; curRound++)
	{
		for (int i = 0; i < rowCount; i++)
		{
			for (int j = 0; j < colCount; j++)
			{
				int neighborCount = 0;

				for (int neighborRow = -1; neighborRow <= 1; neighborRow++)
				{
					for (int neighborCol = -1; neighborCol <= 1; neighborCol++)
					{
						if (i + neighborRow < 0 || i + neighborRow >= rowCount
							|| j + neighborCol < 0 || j + neighborCol >= colCount)
							continue;

						if (neighborRow == 0 && neighborCol == 0)
							continue;

						if (m_map.m_grid[i + neighborRow][j + neighborCol] == 1)
							neighborCount++;

					}
				}

				if (neighborCount > 2 && rand() % 100 > 75)
					m_map.m_grid[i][j] = 1;
			}
		}
	}

	//Erase lone walls
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			int neighborCount = 0;

			for (int neighborRow = -1; neighborRow <= 1; neighborRow++)
			{
				for (int neighborCol = -1; neighborCol <= 1; neighborCol++)
				{
					if (i + neighborRow < 0 || i + neighborRow >= rowCount
						|| j + neighborCol < 0 || j + neighborCol >= colCount)
						continue;

					if (neighborRow == 0 && neighborCol == 0)
						continue;

					if (m_map.m_grid[i + neighborRow][j + neighborCol] == 1)
						neighborCount++;

				}
			}

			if (neighborCount == 0)
				m_map.m_grid[i][j] = 0;
		}
	}

	//Create some random trees
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			int neighborCount = 0;

			for (int neighborRow = -4; neighborRow <= 4; neighborRow++)
			{
				for (int neighborCol = -4; neighborCol <= 4; neighborCol++)
				{
					if (i + neighborRow < 0 || i + neighborRow >= rowCount
						|| j + neighborCol < 0 || j + neighborCol >= colCount)
						continue;

					if (neighborRow == 0 && neighborCol == 0)
						continue;

					if (m_map.m_grid[i + neighborRow][j + neighborCol] == 1 || m_map.m_grid[i + neighborRow][j + neighborCol] == 2)
						neighborCount++;


				}
			}

			if (neighborCount <= 1 && rand() % 100 > 60)
				m_map.m_grid[i][j] = 2;
		}
	}
}

