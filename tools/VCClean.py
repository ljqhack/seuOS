__author__ = 'nxp39054'
import os
import os.path as path
g_nKeep = 0
def DelTree(treeName, isDelDir=False, isDelRoot=False, lstKeep = []):
    'delete a tree, recursively, it can be non empty!'
    global g_nKeep
    if not path.exists(treeName):
        if not isDelDir and not isDelRoot:
            os.mkdir(treeName)
        return -1L

    for root, dirs, files in os.walk(treeName, topdown=False):
        for name in files:
            sExt = path.splitext(name)[1].lower()
            isToKeep = False
            for s in lstKeep:
                if s == sExt:
                    g_nKeep += 1
                    isToKeep = True
                    break
            if not isToKeep:
                os.remove(path.join(root, name))
            else:
                print "%d, keep file %s" % (g_nKeep, name)
            # print "deleting file %s" % name
        if isDelDir == True:
            for name in dirs:
                dirFull = path.join(root, name)
                lstDir = os.listdir(dirFull)
                if lstDir == []:
                    os.rmdir(path.join(root, name))
                # print "deleting folder %s" % name
    if isDelDir == True and isDelRoot == True and g_nKeep == 0:
        os.rmdir(treeName)


def DistCleanFile(treeName):
    'delete a tree, recursively, it can be non empty!'
    dict = {}
    dict['.aps'] = 1
    dict['.bak'] = 1 ; dict['.bsc'] = 1; dict['.res'] = 1
    dict['.ilk'] = 1
    dict['.ipch'] = 1 ; dict['.lastbuildstate'] = 1
    dict['.map'] = 1 ; dict['.manifest'] = 1 ; dict['.obj'] = 1;
    dict['.pch'] = 1 ; dict['.pdb'] = 1 ; dict['.sbr'] = 1;
    dict['.sdf'] = 1 ; dict['.obj'] = 1 ; dict['.tlog'] = 1 ;
    dict['.idb'] = 1;
    lstKeys = dict.keys()
    if not path.exists(treeName):
        return -1L
    for root, dirs, files in os.walk(treeName, topdown=False):
        for name in files:
            name = name.lower()
            sExt = path.splitext(name)[1]
            for sKey in lstKeys:
                if (sKey == '.uvgui' and name.find(sKey) >= 0) or sKey == sExt:
                    print 'Deleting File: ' + name
                    os.remove(path.join(root, name))
                    break
            # print "deleting file %s" % namE
def MainFunc(sWorkDir):
    DistCleanFile(sWorkDir)
    for root, dirs, files in os.walk(sWorkDir, topdown=False):
        for name in dirs:
            name = name.lower()
            if name == 'debug' or name == 'release' or name == 'sim' or name == 'ipch':
                print "Deleting Dir :" + name
                DelTree(path.join(root,name), True, True)

			
if __name__ == '__main__':
    sWorkDir = path.abspath('.')
    MainFunc(sWorkDir)

