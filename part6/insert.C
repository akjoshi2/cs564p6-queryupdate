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
	int totalRecLen;
	for (int i = 0; i < relAttrCnt; i++)
	{
		totalRecLen += relAttrs[i].attrLen;
	}

  	char outputData[totalRecLen];
    Record outputRec;
    outputRec.data = (void *) outputData;
    outputRec.length = totalRecLen;
	for (int i = 0; i < attrCnt; i++)
	{
		for (int j = 0; j < relAttrCnt; j++)
		{
			if (strcmp(relAttrs[j].attrName,attrList[i].attrName) == 0)
			{
				if (attrList[i].attrValue == NULL)
				{
					return ATTRTYPEMISMATCH;
				}
				char* val;
				float flt;
				int intgr;
				switch(attrList[i].attrType)
				{
					case INTEGER:
						intgr = atoi(attrList[i].attrValue);
						val = (char*)&intgr;
						break;
					case FLOAT:
						flt = atof(attrList[i].attrValue);
						val = (char*)&flt;
						break;
					case STRING:
						val = (char*)attrList[i].attrValue;
						break;
				}
				memcpy(outputData + relAttrs[j].attrOffset, val, relAttrs[j].attrLen);
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

