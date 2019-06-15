#pragma once
#include "Sprite.h"
#include <iostream>

class Map
{
public:
	//Singleton Instance
	static void CreateMap(int height, int width,int cellHeight, int cellWidth) { m_map = Map(height, width, cellHeight,cellWidth); ProceduralGeneration(); }
	static void ProceduralGeneration();
	static Map m_map;
	
//Members and ctor
protected:
	int ** m_grid;
	Sprite*** m_spriteGrid;
	int m_width, m_height;
	int m_cellWidth, m_cellHeight;
	Map(int height, int width, int cellHeight, int cellWidth) :m_width(width), m_height(height), m_cellWidth(cellWidth), m_cellHeight(cellHeight) { m_grid = (int**)calloc(height, sizeof(int*)); m_spriteGrid = (Sprite***)calloc(height, sizeof(Sprite**)); };

//Methods
public:
	static void SetGrid(int ** grid) { m_map.m_grid = grid; }
	static void SetGridCell(int yIndex, int xIndex, int value) { m_map.m_grid[yIndex][xIndex] = value; }
	static void SetSpriteGridCell(int yIndex, int xIndex, Sprite* sprite) { m_map.m_spriteGrid[yIndex][xIndex] = sprite; }
	static void RemoveSpriteGridCell(int yIndex, int xIndex) { m_map.m_spriteGrid[yIndex][xIndex] = NULL; }
	static const int& GetGridCell(int yIndex,int xIndex) { return m_map.m_grid[yIndex][xIndex]; }
	static const int& GetHeight() { return m_map.m_height; }
	static const int& GetWidth() { return m_map.m_width; }
	static const int& GetCellHeight() { return m_map.m_cellHeight; }
	static const int& GetCellWidth() { return m_map.m_cellWidth; }
	static Sprite* GetSpriteCell(int y, int x) 
	{ 
		//Cap the width and height
		if (x < 0)
			x = 0;
		else if (x >= m_map.m_width)
			x = m_map.m_width - 1;
		if (y < 0)
			y = 0;
		else if (x >= m_map.m_height)
			x = m_map.m_height - 1;

		if(m_map.m_spriteGrid[y][x]!=NULL)
			return m_map.m_spriteGrid[y][x];
		return NULL;
	}

};