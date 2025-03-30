import sys
file = open(sys.argv[1], 'r')
lines = file.readlines()
defs = {}
insts_8 = {
    'mov': '00',
    'add': '01',
    'sub': '02',
    'mul': '03',
    'div': '04',
    'sll': '05',
    'srl': '06',
    'l': '07',
    'st': '08',
    'and': '09',
    'or': '0A',
    'xor': '0B',
    'inc': '0C',
    'dec': '0D',
    'cmp': '0E',
}
insts_16 = {
    'mov': '16',
    'add': '17',
    'sub': '18',
    'mul': '19',
    'div': '1A',
    'sll': '1B',
    'srl': '1C',
    'l': '1D',
    'st': '1E',
    'and': '1F',
    'or': '20',
    'xor': '21',
    'inc': '22',
    'dec': '23',
    'cmp': '24',
}
insts_imm8 = {
    'mov': '30',
    'add': '31',
    'sub': '32',
    'mul': '33',
    'div': '34',
    'sll': '35',
    'srl': '36',
    'l': '37',
    'st': '38',
    'and': '39',
    'or': '3A',
    'xor': '3B',
    'inc': '3C',
    'dec': '3D',
    'cmp': '3E',
}
insts_imm16 = {
    'mov': '40',
    'add': '41',
    'sub': '42',
    'mul': '43',
    'div': '44',
    'sll': '45',
    'srl': '46',
    'l': '47',
    'st': '48',
    'and': '49',
    'or': '4A',
    'xor': '4B',
    'inc': '4C',
    'dec': '4D',
    'cmp': '4E',
}
stack8 = {
    'push': '50',
    'pop': '51',
}
stack16 = {
    'push': '52',
    'pop': '53',
    'pushlr': '54',
    'poppc': '55',
}
branch = {
    'jmp': '0F',
    'jz': '10',
    'jnz': '11',
    'jg': '12',
    'jl': '13',
    'jge': '14',
    'jle': '15',
    'b': '60',
    'bl': '61',
}
syscall = {
    'syscall': '70',
}
syscall_s = {
    'print': '00',
    'set_pixel': '01',
    'plot_line': '02',
    'menu': '03',
    'menu2': '04',
    'check_buttons': '05',
    'render': '06',
}
misc = {
    'break': 'FF',
    'rt': '62',
}

first_pass = []

for a in lines:
    line = a.lower().strip()
    parts = line.split()
    inst = '0'
    hreg = '0'
    reg1 = '0'
    reg2 = '0'
    
    first = parts[0] if len(parts) > 0 else ''
    second = parts[1] if len(parts) > 1 else ''
    third = parts[2].replace('\n', '') if len(parts) > 2 else ''
    if first.startswith('/') or first == '\n' or first == '0' or first == '':
        continue
    if third in defs:
        third = defs[third]
    if first == 'define':
        defs[second] = third.replace('h', '')


    if second.startswith('r'):
        reg1 = second[1]
    else:
        if second.startswith('er'):
            reg1 = second[2]

    if third.startswith('#'):
        hreg = third[1:].replace('\n', '').replace('h', '')
    elif third.startswith('r'):
        reg2 = third[1]
    else:
        if third.startswith('er'):
            reg2 = third[2]
        elif third in defs:
            hreg = defs[third]

    if second.startswith('r'):
        if third.startswith('#'):
            inst = insts_imm8[first]
        else:
            inst = insts_8[first] if first not in stack8 else stack8[first]
    elif second.startswith('er'):
        if third.startswith('#'):
            inst = insts_imm16[first]
        elif first in branch:
            if first in branch:
                inst = branch[first]
                reg1 = second[2]
        else:
            inst = insts_16[first]if first not in stack16 else stack16[first]
    if first in stack16:
        inst = stack16[first]
    if first in syscall:
        inst = syscall[first]
        hreg = syscall_s[second]
    if first in misc:
        inst = misc[first]
    if first == 'hex':
        second = second.ljust((len(second) + 3) // 4 * 4, '0')
        b = [second[i:i+4] for i in range(0, len(second), 4)]

    #print(a)
    b = ''
    if len(hreg) > 1:
        b = inst + reg1 + reg2 + '|' + hreg.zfill(4)
    elif len(hreg) == 1:
        b = inst + reg1 + reg2
    if len(b) < 4:
        b = first.replace(':','') + '_address/definition_'
    if len(hreg) > 1:
        first_pass.append(b.split('|')[0])
        first_pass.append(b.split('|')[1])
    else:
        first_pass.append(b)
    #print('\n\n')
count = 0
addresses = {}
second_pass = []
for line in first_pass:
    if line.endswith('_address/definition_'):
        addresses[line.replace('_address/definition_', '')] = count
        continue
    second_pass.append(line)
    
    count += 2
final = []
for line in second_pass:
    if line in addresses:
        line = f"{addresses[line]:X}".zfill(4)
        #print('\n'+line+'\n')
    final.append(line)
with open(sys.argv[2], "wb") as f:
    for hex_str in final:
        # Swap the first two characters and move them to the end of the string
        swapped_hex_str = hex_str[2:] + hex_str[:2]
        binary_data = bytes.fromhex(swapped_hex_str)
        f.write(binary_data)

file.close()