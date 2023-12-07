#include "catalog.h"
#include "query.h"


/*
 * Inserts a record into the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Insert(const string & relation, 
	const int attrCnt, 
	const attrInfo attrList[])
{
	Status status;
	int relAttrCnt;
	AttrDesc* relAttrs;
	InsertFileScan rel(relation, status);
    if (status != OK)
	{
		return status;
	}
	status = attrCat->getRelInfo(relation, relAttrCnt, relAttrs);
	if (status != OK)
	{ 
		return status;
	}
	if (relAttrCnt != attrCnt)
	{
		return ATTRTYPEMISMATCH;
	}
	int totalRecLen;
	for (int i = 0; i < relAttrCnt; i++)
	{
		totalRecLen += relAttrs[i].attrLen;
	}

  	char outputData[totalRecLen];
    Record outputRec;
    outputRec.data = (void *) outputData;
    outputRec.length = totalRecLen;
	for (int i = 0; i < relAttrCnt; i++)
	{
		for (int j = 0; j < attrCnt; j++)
		{
			if (strcmp(relAttrs[i].attrName,attrList[j].attrName) == 0)
			{
				if (attrList[j].attrValue == NULL)
				{
					return ATTRTYPEMISMATCH;
				}
				char* val;
				float flt;
				int intgr;
				switch(relAttrs[i].attrType)
				{
					case INTEGER:
						intgr = atoi(attrList[j].attrValue);
						val = (char*)&intgr;
						break;
					case FLOAT:
						flt = atof(attrList[j].attrValue);
						val = (char*)&flt;
						break;
					case STRING:
						val = (char*)attrList[j].attrValue;
						break;
				}
				memcpy(outputData + relAttrs[i].attrOffset, val, relAttrs[i].attrLen);
			}
		}
	}
	RID outRID;
	status = rel.insertRecord(outputRec, outRID);
	if (status != OK)
	{
		return status;
	}
	return OK;
}

