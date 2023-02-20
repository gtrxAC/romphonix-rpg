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

def append16(value):
    global out
    out.append(value & 0xFF)
    out.append((value & 0xFF00) >> 8)

with open('phones.json') as file:
    phones = json.loads(file.read())
    append16(len(phones))
    for p in phones:
        appendStr(p['brand'], 16)
        appendStr(p['model'], 16)
        appendStr(p['description'], 128)

        append16(p['year'])
        out.append(p['fictional'])

        append16(p['battery'])
        append16(p['attack'])
        append16(p['defense'])
        append16(p['weight'])
        out.append(p['rarity'])
        out.append(p['brokenChance'])
        out.append(p['baseExp'])
        out.append(icons[p['icon']])
        
with open('phones.tfs', 'wb') as outfile:
    outfile.write(out)
    outfile.close()