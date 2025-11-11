#!/usre/bin/env python3

from libgenerate.generate_command import *
from libgenerate.generate_payload import *
from libgenerate.generate_doc import *

def openFiles():
    generateCommonHeaderFilesOpen()
    generateDocumentationReadme()

def closeFiles():
    generateCommonHeaderFilesClose()

def add(commandId, commandName, 
                 commandPayloadByteNames, 
                 responsePayloadByteNames):

    generateCommandFile(commandId, commandName, commandPayloadByteNames, responsePayloadByteNames)
    generatePayloadFile(commandId, commandName, commandPayloadByteNames, responsePayloadByteNames)
    generateDocumentationFile(commandId, commandName, commandPayloadByteNames, responsePayloadByteNames)

