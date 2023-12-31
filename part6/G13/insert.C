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
	// Get relation info
	status = attrCat->getRelInfo(relation, relAttrCnt, relAttrs);
	if (status != OK)
	{ 
		return status;
	}
	// Get total length
	int totalRecLen = 0;
	for (int i = 0; i < relAttrCnt; i++)
	{
		totalRecLen += relAttrs[i].attrLen;
	}

  	char outputData[totalRecLen];
    Record outputRec;
    outputRec.data = (void *) outputData;
    outputRec.length = totalRecLen;
	// Need two loops to ensure the attributes are ordered correctly
	for (int i = 0; i < relAttrCnt; i++)
	{
		for (int j = 0; j < attrCnt; j++)
		{
			if (strcmp(relAttrs[i].attrName,attrList[j].attrName) == 0)
			{
				//If no value is specified for the attribute, reject the insertion
				if (attrList[j].attrValue == NULL)
				{
					return ATTRTYPEMISMATCH;
				}
				char* val;
				float flt;
				int intgr;
				// Handle types
				switch(attrList[j].attrType)
				{
					case INTEGER:
						intgr = atoi((char*)attrList[j].attrValue);
						val = (char*)&intgr;
						break;
					case FLOAT:
						flt = atof((char*)attrList[j].attrValue);
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
	// Insert record
	status = rel.insertRecord(outputRec, outRID);
	if (status != OK)
	{
		return status;
	}
	return OK;
}

