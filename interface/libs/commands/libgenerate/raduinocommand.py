#!/usre/bin/env python3

from libgenerate.generate_command import *
from libgenerate.generate_payload import *
from libgenerate.generate_doc import *

class raduinoCommandGenerator:
    def __init__(self, codePath="include/cmd", docPath="../../../doc/reference-guides/commands/"):
        self.codePath = codePath
        self.docPath = docPath
        self.openFiles()

    def openFiles(self):
        generateCommonHeaderFilesOpen(self.codePath)
        generateDocumentationReadme(self.docPath)

    def closeFiles(self):
        generateCommonHeaderFilesClose(self.codePath)

    def add(self,
            commandId,
            commandName,
            commandPayloadFields,
            responsePayloadFields):

        generateCommandFile(self.codePath, commandId, commandName, commandPayloadFields, responsePayloadFields)
        generatePayloadFile(self.codePath, commandName, commandPayloadFields, responsePayloadFields)
        generateDocumentationFile(self.docPath, commandName, commandPayloadFields, responsePayloadFields)

