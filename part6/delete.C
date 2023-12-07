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
	// return error if any input is null
    if (relation == "" || attrValue == nullptr) {
        return BADSCANPARM; // idk if this is the right error message
    }

    Status status;
    RID rid;
    Record rec;
    AttrDesc attrDesc;

    HeapFileScan hfs(relation, status);
    if (status != OK) { 
        return status;
    }

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


