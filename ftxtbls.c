#include <ttobjs.h>
#include <tttables.h>



int
TT_Get_Num_Tables(TT_Face face)
{
    PFace f;

    f = HANDLE_Face(face);

    return f->numTables;
}



TT_Error
TT_Get_Table_Entry(TT_Face face, int index,
		   ULong *Tag, ULong *CheckSum, ULong *Offset, ULong *Length)
{
    PFace f;

    f = HANDLE_Face(face);

    if (index >= f->numTables)
	return TT_Err_Invalid_Argument;

    if (Tag)
	*Tag = f->dirTables[index].Tag;
    if (CheckSum)
	*CheckSum = f->dirTables[index].CheckSum;
    if (Offset)
	*Offset = f->dirTables[index].Offset;
    if (Length)
	*Length = f->dirTables[index].Length;

    return TT_Err_Ok;
}
