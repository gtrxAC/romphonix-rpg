import json

with open('assets/data/phones.json') as file:
    with open('assets/data/phones.csv', 'w') as csv:
        def writeCol(s):
            csv.write(str(s) + ',')

        csv.write('brand,model,year,battery,attack,defense,weight,rarity,brokenChance,baseExp,icon,description\n')
        phones = json.loads(file.read())
        for p in phones:
            writeCol(p['brand'])
            writeCol(p['model'])
            writeCol(p['year'])
            writeCol(p['battery'])
            writeCol(p['attack'])
            writeCol(p['defense'])
            writeCol(p['weight'])
            writeCol(p['rarity'])
            writeCol(p['brokenChance'])
            writeCol(p['baseExp'])
            writeCol(p['icon'])
            csv.write('"' + p['description'] + '"')
            csv.write('\n')
        csv.close()