#include "NoteMisc.h"

#include <QDir>





bool delDirRecursive(QString dirPath)
{
    QString pathDelNote = dirPath;

    QDir dirNote;
    if(dirNote.exists(pathDelNote))
    {
        QDir             dir(pathDelNote);
        QFileInfoList    fileList;
        QFileInfo        curFile;
        QFileInfoList    fileListTemp;
        int              infoNum,i,j;

        fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                                   |QDir::Readable|QDir::Writable
                                   |QDir::Hidden|QDir::NoDotAndDotDot
                                   ,QDir::Name);

        while(fileList.size() > 0)
        {
            infoNum = fileList.size();

            for(i = infoNum - 1; i >= 0; i--)
            {
                curFile = fileList[i];
                if(curFile.isFile())
                {
                    QFile fileTemp(curFile.filePath());
                    fileTemp.remove();
                    fileList.removeAt(i);
                }

                if(curFile.isDir())
                {
                    QDir dirTemp(curFile.filePath());
                    fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
                                                         | QDir::Readable | QDir::Writable
                                                         | QDir::Hidden | QDir::NoDotAndDotDot
                                                         , QDir::Name);
                    if(fileListTemp.size() == 0)
                    {
                        dirTemp.rmdir(".");
                        fileList.removeAt(i);
                    }
                    else
                    {
                        for(j = 0; j < fileListTemp.size(); j++)
                        {
                            if(!(fileList.contains(fileListTemp[j])))
                            {
                                fileList.append(fileListTemp[j]);
                            }
                        }
                    }
                }
            }

            dir.rmdir(".");
        }

    }

    return true;

}
