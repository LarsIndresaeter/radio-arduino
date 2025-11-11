#!/use/bin/env python3

def snakecaseToCamelCase(variableName):
    result = variableName.capitalize()
    
    idx = result.find("_")
    while idx > 0:
        result = result[:idx] + result[idx + 1:].capitalize()
        idx = result.find("_")

    return result

def arraySizeFromVariableName(variableName):
    result = 1

    try:
        first = variableName.index("[")
        last = variableName.index("]")

        if(first > 0 and last > 0):
            result = int(variableName[first+1:last])
            return result
    except:
        pass

    return result

def arrayBasenameFromVariableName(variableName):
    result = variableName

    try:
        first = variableName.index("[")

        if(first > 0):
            result = variableName[0:first]
            return result
    except:
        pass

    return result

