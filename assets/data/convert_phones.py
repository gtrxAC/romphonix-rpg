import json

icons = {
    "candybar": 0,
    "new_candybar": 1,
    "slider": 2,
    "flip": 3,
    "flipcover": 4,
    "qwerty": 5,
    "gx55x": 6,
    "3650": 7,
    "ngage": 8,
    "p900": 9,
    "communicator": 10,
    "xelibri8": 11,
    "77xx": 12,
    "sx1": 13
}

out = bytearray()

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

with open('phones.json') as file:
    phones = json.loads(file.read())
    append32(len(phones))
    for p in phones:
        appendStr(p['brand'], 16)
        appendStr(p['model'], 16)
        appendStr(p['sprite'], 32)
        appendStr(p['description'], 128)

        append32(p['year'])
        append32(p['fictional'])

        append32(p['battery'])
        append32(p['attack'])
        append32(p['defense'])
        append32(p['weight'])
        append32(p['rarity'])
        append32(p['brokenChance'])
        append32(p['baseExp'])
        append32(icons[p['icon']])
        
with open('phones.tfs', 'wb') as outfile:
    outfile.write(out)
    outfile.close()