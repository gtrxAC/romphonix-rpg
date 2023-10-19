import json

def appendStr(string, length):
    global out
    # -1 because null terminator
    if len(string) > length - 1:
        raise ValueError(f"String '{string}' is too long, must be up to {length - 1} characters. Try to make the text {len(string) - length - 1} characters shorter, or if you really need to, increase the length of the string in json2tfs.py and in the structure in the appropriate src/*.h file.")
    
    out += bytes(string, 'latin-1')
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

with open('assets/data/phones.json') as file:
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
        
with open('assets/data/phones.tfs', 'wb') as outfile:
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
    "self_damage": 2,
    "heal": 3,
    "drain": 4,
    "confuse": 5,
    "atk_up": 6,
    "def_up": 7,
    "accuracy_up": 8,
    "accuracy_down": 9,
    "stomp": 10,
    "flex_rip": 11,
}

animationTargets = {
    "self": 0,
    "enemy": 1
}

def appendEffect(eff):
    if 'parameter' not in eff: eff['parameter'] = 0
    if 'chance' not in eff: eff['chance'] = 100

    append32(skillEffects[eff['type']])
    append32(eff['parameter'])
    append32(eff['chance'])

with open('assets/data/skills.json') as file:
    skills = json.loads(file.read())
    append32(len(skills))
    for s in skills:
        appendStr(s['name'], 16)
        appendStr(s['description'], 128)
        append32(skillTypes[s['skillType']])
        
        if len(s['effects']):
            appendEffect(s['effects'][0])

            if len(s['effects']) > 1:
                appendEffect(s['effects'][1])
            else:
                for i in range(0, 3): append32(0)
        else:
            for i in range(0, 6): append32(0)

        appendStr(s['learnByPhones'], 256)
        appendStr(s['animation'], 64)
        append32(animationTargets[s['animationTarget']])
        
with open('assets/data/skills.tfs', 'wb') as outfile:
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
    "combine": 4,
    "skill": 5
}

bagPockets = {
    "cards": 0,
    "items": 1,
    "healing": 2
}

with open('assets/data/items.json') as file:
    items = json.loads(file.read())
    append32(len(items))
    for i in items:
        appendStr(i['name'], 32)
        appendStr(i['description'], 128)
        append32(itemEffects[i['effect']])
        append32(i['effectParameter'])
        appendStr(i['sprite'], 32)
        append32(bagPockets[i['pocket']])
        
with open('assets/data/items.tfs', 'wb') as outfile:
    outfile.write(out)
    outfile.close()