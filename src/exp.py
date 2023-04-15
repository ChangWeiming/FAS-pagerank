import os

os.system("g++ -g -Wall -o page_rank.exe page_rank.cpp utils.cpp")
os.system("g++ -g -Wall -o GreedyFAS_final.exe GreedyFAS_final.cpp")
os.system("g++ -g -Wall -o SortFAS_final.exe sortFAS_final.cpp")

rootdir = './dataset'
for dirpath, dirnames, filenames in os.walk(rootdir):
    for filename in filenames:
        if filename.split('.')[-1] != "txt":
            continue
        n = filename.split("_")[1]
        rFiles = ['page_rank', 'GreedyFAS_final', 'SortFAS_final']
        for f in rFiles:
            inPath = os.path.join(dirpath, filename)
            outPath = os.path.join('./result', f + '_' + filename)
        #    print('cmd:', './{}.exe {} {} >> {}'.format(f, n, inPath, outPath))
            os.system('{}.exe {} {} >> {}'.format(f, n, inPath, outPath))