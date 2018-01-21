circle = OxProgram.new("circle.ptx", OxProgramSource.file, "__ox_intersect__")
print(circle:isValid())
print(circle:getUniqueIdentifier():toString())
print("Alive entities:"..ox_alive_entities())