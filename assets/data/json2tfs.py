import json

def appendStr(string, length):
    global out
    out += bytes(string, 'ascii')
    for i in range(length - len(string)): out.append(0)

def append32(value):
    global out
    out.append(value & 0xFF)
    out.append((value & 0xFF00) >> 8)
    out.append((value & 0xFF0000) >> 16)
    out.append((value & 0xFF000000) >> 24)

# ______________________________________________________________________________
#
#  Convert phones
#  Should match the structure in src/phones.h (PhoneSpecs)
# ______________________________________________________________________________
#
icons = {
    "candybar": 0,
    "new_candybar": 1,
    "slider": 2,
    "flip": 3,
    "flipcover": 4,
    "qwerty": 5,
    "3650": 6,
    "ngage": 7,
    "p900": 8,
    "communicator": 9,
    "xelibri8": 10,
    "77xx": 11,
    "sx1": 12,
    "touch": 13,
    "qwerty_slider": 14
}

out = bytearray()

with open('phones.json') as file:
    phones = json.loads(file.read())
    append32(len(phones))
    for i in phones:
        appendStr(i['brand'], 16)
        appendStr(i['model'], 16)
        appendStr(i['sprite'], 32)
        appendStr(i['description'], 128)
        append32(i['year'])

        append32(i['battery'])
        append32(i['attack'])
        append32(i['defense'])
        append32(i['weight'])
        append32(i['rarity'])
        append32(i['brokenChance'])
        append32(i['baseExp'])
        append32(icons[i['icon']])
        
with open('phones.tfs', 'wb') as outfile:
    outfile.write(out)
    outfile.close()

# ______________________________________________________________________________
#
#  Convert skills
#  Should match the structure in src/skills.h (SkillSpecs)
# ______________________________________________________________________________
#
out = bytearray()

skillTypes = {
    "screen": 0,
    "board": 1,
    "cover": 2,
    "battery": 3,
}

skillEffects = {
    "none": 0,
    "damage": 1,
    "drain": 2,
    "confuse": 3,
    "atk_up": 4,
    "def_up": 5
}

with open('skills.json') as file:
    skills = json.loads(file.read())
    append32(len(skills))
    for s in skills:
        appendStr(s['name'], 16)
        appendStr(s['description'], 128)
        append32(skillTypes[s['skillType']])
        append32(skillEffects[s['effect']])
        append32(s['effectParameter'])
        append32(skillEffects[s['secondaryEffect']])
        append32(s['secondaryEffectParameter'])
        appendStr(s['learnByPhones'], 256)
        appendStr(s['animation'], 64)
        
with open('skills.tfs', 'wb') as outfile:
    outfile.write(out)
    outfile.close()

# ______________________________________________________________________________
#
#  Convert items
#  Should match the structure in src/items.h (ItemSpecs)
# ______________________________________________________________________________
#
out = bytearray()

itemEffects = {
    "heal": 0,
    "revive": 1,
    "upgrade": 2,
    "repair": 3,
    "combine": 4
}

with open('items.json') as file:
    items = json.loads(file.read())
    append32(len(items))
    for i in items:
        appendStr(i['name'], 32)
        appendStr(i['description'], 128)
        append32(itemEffects[i['effect']])
        append32(i['effectParameter'])
        appendStr(i['sprite'], 32)
        append32(i['pocket'])
        
with open('items.tfs', 'wb') as outfile:
    outfile.write(out)
    outfile.close()