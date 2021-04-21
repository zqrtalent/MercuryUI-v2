#pragma once
#ifndef CGRIDDEFINES_H
#define CGRIDDEFINES_H

#define GRID_VERSION				0x0105	// version 1.5

#define DEFAULT_EDIT_LIMIT			100
#define DEFAULT_BUFFER_LENGTH		255
#define	DEFAULT_TIMER_INCSEARCH		2500	// incremental search reset period (default 2.5 sec)

#define BORDER_STYLE_LEFT			2
#define BORDER_STYLE_TOP			4
#define BORDER_STYLE_RIGHT			8
#define BORDER_STYLE_BOTTOM			16

#define ROW							(int)mDraw.cd->nmcd.dwItemSpec
#define COL							mDraw.cd->iSubItem

#define MiddleInt(i1, i2)	(((i1) + (i2)) / 2)
#define RHEIGHT(pRect)		((pRect)->bottom - (pRect)->top)
#define RWIDTH(pRect)		((pRect)->right - (pRect)->left)
#define DC					mDraw.cd->nmcd.hdc

// custom drawing settings
#define COMBO_BTNWIDTH		14
#define CUSTOM_BTNWIDTH		14
#define CHECK_BTNWIDTH		16
#define CHECK_MARKWIDTH		14

//
//	enum CellType
//
enum CellType
	{
	CELL_REGULAR = 0,		// regular cell (text (and image) only)
	CELL_CUSTOMEDITOR,		// editable cell (the OnCustomCellClick callback is called)
	CELL_EDITBOX,			// editable cell (edit box)
	CELL_COMBOBOX,			// editable cell (drop-down-list combo box)
	CELL_EDITCOMBOBOX,		// editable cell (drop-down combo box)
	CELL_CHECKBOXON,		// editable cell (check box ON)
	CELL_CHECKBOXOFF,		// editable cell (check box OFF)
	CELL_SEPARATOR			// not editable cell
	};

//
//	enum FontStyle
//
enum FontStyle
	{
	FONT_BOLD		= 1,
	FONT_ITALIC		= 1 << 1,
	FONT_UNDERLINE	= 1 << 2,
	FONT_STRIKEOUT	= 1 << 3
	};
#endif //CGRIDDEFINES_H