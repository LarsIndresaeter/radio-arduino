#!/usre/bin/env python3

from libgenerate.generate_command import *
from libgenerate.generate_payload import *
from libgenerate.generate_doc import *

def openFiles():
    generateCommonHeaderFilesOpen()
    generateDocumentationReadme()

def closeFiles():
    generateCommonHeaderFilesClose()

def add(commandId,
        commandName,
        commandPayloadFields,
        responsePayloadFields):

    generateCommandFile(commandId, commandName, commandPayloadFields, responsePayloadFields)
    generatePayloadFile(commandId, commandName, commandPayloadFields, responsePayloadFields)
    generateDocumentationFile(commandId, commandName, commandPayloadFields, responsePayloadFields)

