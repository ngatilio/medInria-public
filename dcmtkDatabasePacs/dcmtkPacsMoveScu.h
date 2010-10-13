#ifndef DCMTKPACSMOVESCU_H
#define DCMTKPACSMOVESCU_H

#include "dcmtkMoveScu.h"
#include <medPacs/medAbstractPacsMoveScu.h>

class dcmtkPacsMoveScu : public medAbstractPacsMoveScu
{
    Q_OBJECT

public:
    virtual QString description(void) const;
    static bool     registered(void);

    dcmtkPacsMoveScu();

    int sendMoveRequest(const char* peerTitle, const char* peerIP, unsigned int peerPort, 
                        const char* ourTitle, const char* ourIP, unsigned int ourPort);


    bool addQueryAttribute(int group, int elem, const char* value);

    void setQueryLevel(eQueryLevel level);

    void clearAllQueryAttributes();

    void useBuildInStoreSCP(bool flag);

    bool setStorageDirectory(const char* directory);

private:
    dcmtkMoveScu scu;

};

medAbstractPacsMoveScu *createDcmtkMoveScu(void); 

#endif //DCMTKPACSMOVESCU_H
