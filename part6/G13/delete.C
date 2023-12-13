#include "catalog.h"
#include "query.h"


/*
 * Deletes records from a specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Delete(const string & relation, 
		       const string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue)
{
    Status status;
    RID rid;
    AttrDesc attrDesc;

    HeapFileScan hfs(relation, status);
    if (status != OK) { 
        return status;
    }
    if (attrName.length() == 0)
    {
        //If no attribute given then scan through everything
        status = hfs.startScan(0,0,STRING,NULL,EQ);
    }
    else
    {
        //  get the attribute description
        status = attrCat->getInfo(relation, attrName, attrDesc);
        if (status != OK) { 
            return status; 
        }

        const char* filter;
        if (type == INTEGER) {
            filter = (char*) new int(atoi(attrValue));
        } else if (type == STRING) {
            filter = attrValue;
        } else {
            filter = (char*) new float(atof(attrValue));
        }
        // scan through the heapfile with matching type, attrValue, and op
        status = hfs.startScan(attrDesc.attrOffset, attrDesc.attrLen, type, filter, op);
    }
    if (status != OK) return status;

    // delete tuples
    while(hfs.scanNext(rid) == OK) {
        status = hfs.deleteRecord();
        if (status != OK) {
            return status;
        }
    }

    // close the scan
    hfs.endScan();

    return OK;
}


