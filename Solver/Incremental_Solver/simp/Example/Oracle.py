import argparse
import os
import re
from subprocess import call
import sys

__author__ = 'xiangyuzhang'


def transINV(lineIn, lineOut):
    inV = str(lineIn[0])
    outV = str(lineOut)
    cnfLines = []
    cnfLines.append(inV + ' ' + outV + ' 0\n')
    cnfLines.append('-' + inV + ' -' + outV + ' 0\n')
    return cnfLines


def transAND(lineIn, lineOut):
    cnfLines = []
    outV = str(lineOut)
    firstLine = ''
    for inV in lineIn:
        inVstr = str(inV)
        firstLine += '-' + inVstr + ' '
        cnfLines.append(inVstr + ' -' + outV + ' 0\n')
    firstLine += outV + ' 0\n'
    cnfLines.insert(0, firstLine)
    return cnfLines


def transOR(lineIn, lineOut):
    cnfLines = []
    outV = str(lineOut)
    firstLine = ''
    for inV in lineIn:
        inVstr = str(inV)
        firstLine += inVstr + ' '
        cnfLines.append('-' + inVstr + ' ' + outV + ' 0\n')
    firstLine += '-' + outV + ' 0\n'
    cnfLines.insert(0, firstLine)
    return cnfLines


def transXOR(lineIn, lineOut):
    inV1 = str(lineIn[0])
    inV2 = str(lineIn[1])
    outV = str(lineOut)
    cnfLines = []
    cnfLines.append('-' + inV1 + ' -' + inV2 + ' -' + outV + ' 0\n')
    cnfLines.append(inV1 + ' ' + inV2 + ' -' + outV + ' 0\n')
    cnfLines.append(inV1 + ' -' + inV2 + ' ' + outV + ' 0\n')
    cnfLines.append('-' + inV1 + ' ' + inV2 + ' ' + outV + ' 0\n')
    return cnfLines


def transNOR(lineIn, lineOut):
    cnfLines = []
    outV = str(lineOut)
    firstLine = ''
    for inV in lineIn:
        inVstr = str(inV)
        firstLine += inVstr + ' '
        cnfLines.append('-' + inVstr + ' -' + outV + ' 0\n')
    firstLine += outV + ' 0\n'
    cnfLines.insert(0, firstLine)
    return cnfLines


def transNAND(lineIn, lineOut):
    cnfLines = []
    outV = str(lineOut)
    firstLine = ''
    for inV in lineIn:
        inVstr = str(inV)
        firstLine += '-' + inVstr + ' '
        cnfLines.append(inVstr + ' ' + outV + ' 0\n')
    firstLine += '-' + outV + ' 0\n'
    cnfLines.insert(0, firstLine)
    return cnfLines


def transBUF(lineIn, lineOut):
    inV = str(lineIn[0])
    outV = str(lineOut)
    cnfLines = []
    cnfLines.append(inV + ' -' + outV + ' 0\n')
    cnfLines.append('-' + inV + ' ' + outV + ' 0\n')
    return cnfLines


def transZERO(lineIn, lineOut):
    cnfLines = []
    cnfLines.append('-' + str(lineOut) + ' 0\n')
    return cnfLines


def transONE(lineIn, lineOut):
    cnfLines = []
    cnfLines.append(str(lineOut) + ' 0\n')
    return cnfLines


class Oracle:
    def __init__(self, path, PI_path, PO_path):
        self.Oralce_path = path
        self.PI_path = PI_path
        self.PO_path = PO_path
        self.indexVarDict = {}
        self.varIndexDict = {}

        self.PI_temp = {}
        self.PO_temp = {}

        self.pisIndex = []
        self.posIndex = []

        self.cnFile = []
        self.assign = []
        self.final = []

    def process(self):
        self.__oracV2cnf()
        self.__parse_PI()
        self.__assign()
        self.__solve()
        self.__grab()
        self.__export()

    def __parse_PI(self):
        with open(self.PI_path, "r") as infile:
            lines = infile.readlines()
        PI_names = lines[0].replace("\n","").split("\t")[:-1]
        PI_values = lines[1].replace("\n","").split("\t")
        for index in range(0, len(PI_names)):
            if PI_names[index] != "CONST1" and PI_names[index] != "CONST0":
                self.PI_temp[self.varIndexDict[PI_names[index]]] = PI_values[index]

    def __assign(self):
        for key in self.PI_temp.keys():
            if self.PI_temp[key] is "1":
                self.assign.append(str(key) + " 0\n")
            elif self.PI_temp[key] is "0":
                self.assign.append("-" + str(key) + " 0\n")
        print ""

    def __solve(self):
        self.final = self.cnFile + self.assign
        outxt = ("").join(self.final)
        with open("cnf_to_solve", "w") as outfile:
            outfile.write(outxt)

        cmmd = "minisat cnf_to_solve solution"
        call(cmmd, shell=True)

    def __grab(self):
        sat = False
        with open("solution", "r") as infile:
            lines = infile.read().split("\n")
        if lines[0] == "SAT":
            sat = True
        if sat == True:
            nodeVals = lines[1].split()
            for node in self.posIndex:
                if "-" in nodeVals[node - 1]:
                    self.PO_temp[node] = "0"
                else:
                    self.PO_temp[node] = "1"

    def __export(self):
        with open(self.PO_path, "w") as outfile:
            for key in self.PO_temp:
                outfile.write(self.indexVarDict[key] + "\t")
            outfile.write("\n")
            for key in self.PO_temp:
                outfile.write(self.PO_temp[key] + "\t")
            outfile.write("\n")

    def __oracV2cnf(self):

        cnfLines = []
        gateTypeDict = {'and': 1, 'or': 2, 'xor': 3, 'inv': 4, 'buf': 5, 'nand': 6, 'nor': 7, 'one': 8, 'zero': 9}
        options = {1: transAND, 2: transOR, 3: transXOR, 4: transINV, 5: transBUF, 6: transNAND, 7: transNOR,
                   8: transONE, 9: transZERO}

        with open(self.Oralce_path, 'r') as infile:
            inV = infile.read()
            Vlines = inV.split(';\n')

        varIndex = 1


        # order of integers: PI vars, programming bit vars, PO vars, internal wire vars;
        for line in Vlines:
            line = line.replace('\n', '')
            if 'input' in line and not '//' in line:
                # print line
                PIs = re.search(r'(?<=input )(.*)(?=$)', line).group().replace(' ', '').split(',')
                tmpPis = []
                for pi in PIs:
                    pi = pi.replace('\\', '').replace('[', '').replace(']', '')
                    self.varIndexDict[pi] = varIndex
                    self.indexVarDict[varIndex] = pi
                    self.pisIndex.append(varIndex)
                    varIndex += 1

            elif 'output' in line and not '//' in line:
                POs = re.search(r'(?<=output )(.*)(?=$)', line).group().replace(' ', '').split(',')
                for po in POs:
                    po = po.replace('\\', '').replace('[', '').replace(']', '')
                    self.varIndexDict[po] = varIndex
                    self.indexVarDict[varIndex] = po
                    self.posIndex.append(varIndex)
                    # poVars.append(po)
                    varIndex += 1

            elif 'wire' in line and not '//' in line:
                wires = re.search(r'(?<=wire )(.*)(?=$)', line).group().replace(' ', '').split(',')
                for w in wires:
                    self.varIndexDict[w] = varIndex
                    self.indexVarDict[varIndex] = w
                    varIndex += 1
            elif line != '' and line[0] != '/' and not 'module' in line:
                line = line.replace(' ', '')
                if '.' in line and '(' in line:  # means it is a mapped Verilog
                    gate = re.search(r'^(.*)(?=g\S+\(\.)', line).group().strip('1234567890')
                # convert vars to standard form:
                buf_split = [term.replace('\\', '').replace('[', '').replace(']', '') for term in line.split('.')]
                buf_processed = []
                for i in range(1, len(buf_split)):
                    buf_processed.append(re.search(r'\((.*)\)', buf_split[i]).group().strip('( )'))
                # convert standard vars to integer indexes in cnf file:
                lineOut = self.varIndexDict[buf_processed[-1]]
                lineIn = []
                for i in range(len(buf_processed) - 1):
                    lineIn.append(self.varIndexDict[buf_processed[i]])
                # convert logic gate to CNF format:
                caseNo = gateTypeDict[gate]
                cnfLines = options[caseNo](lineIn, lineOut)
                for line in cnfLines:
                    self.cnFile.append(line)

        varNum = varIndex - 1
        clauseNum = len(self.cnFile) + len(self.pisIndex)
        cmmtLine1 = 'c This file is generated by oracV2cnf\n'
        firstLine = 'p cnf ' + str(varNum) + ' ' + str(clauseNum) + '\n'
        self.cnFile.insert(0, firstLine)
        self.cnFile.insert(0, cmmtLine1)

        outxt = ''.join(self.cnFile)
        with open("Orac_cnf", 'w') as outfile:
            outfile.write(outxt)


def runmain():
    parser = argparse.ArgumentParser(usage='python Oracle.py <orac.v> <PI.txt> [-o <PO>]]',
                                     description='This program will be used to queryOrac', )
    parser.add_argument('<orac.v>', help='input oracle Verilog file that defines the function of the circuit')
    parser.add_argument('<PI>', help='the PI value that we want to assign')
    parser.add_argument('<PO>', help='the PO value that we will export')

    outfile = sys.argv[3]

    orcIn = sys.argv[1]
    OracInPath = os.path.abspath(orcIn)

    if not os.path.isfile(OracInPath):
        print 'Invalid oracle circuit file!!!!\n'
        return
    PI = sys.argv[2]
    PI_path = os.path.abspath(PI)

    if not os.path.isfile(PI_path):
        print 'Invalid PI circuit file!!!!\n'
        return
    ORA = Oracle(orcIn, PI_path, outfile)
    ORA.process()


if __name__ == '__main__':
    runmain()
