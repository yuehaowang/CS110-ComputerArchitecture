import random, sys

MARK_ROAD = ' '
MARK_WALL = '#'
MARK_START = '@'
MARK_GOAL = '%'


def gen_maze(m, w, h, d):
    num_steps = int(w * h * (1-d))

    visited_set = [[False] * w for _ in range(h)]
    visited_num = 0
    stack = []
    
    start = (random.randint(0, w - 1), random.randint(0, h - 1))
    m[start[1]][start[0]] = MARK_START
    cur = start
    visited_set[cur[1]][cur[0]] = True
    road_list = []

    while visited_num < num_steps:
        directions = list(range(4))  # top: 0, left: 1, bottom: 2, right: 3
        if cur[0] == 0 or visited_set[cur[1]][cur[0] - 1]:
            directions.remove(1)
        if cur[1] == 0 or visited_set[cur[1] - 1][cur[0]]:
            directions.remove(0)
        if cur[0] == w - 1 or visited_set[cur[1]][cur[0] + 1]:
            directions.remove(3)
        if cur[1] == h - 1 or visited_set[cur[1] + 1][cur[0]]:
            directions.remove(2)

        if len(directions) != 0:
            temp = None
            direction = random.choice(directions)
            if direction == 0:
                temp = (cur[0], cur[1] - 1)
            elif direction == 1:
                temp = (cur[0] - 1, cur[1])
            elif direction == 2:
                temp = (cur[0], cur[1] + 1)
            elif direction == 3:
                temp = (cur[0] + 1, cur[1])
            
            stack.append(cur)
            m[temp[1]][temp[0]] = MARK_ROAD

            cur = temp
            visited_set[cur[1]][cur[0]] = True
            visited_num += 1
            road_list.append(cur)

        elif len(stack) != 0:
            cur = stack.pop()
    
    goal = random.choice(road_list)
    m[goal[1]][goal[0]] = MARK_GOAL

def maze_to_str(m, w, h):
    res = '%s %s\n' % (w, h)
    
    for row in m:
        for o in row:
            res += o
        res += '\n'

    return res

def display_maze(m, w, h):
    print('%s %s' % (w, h))
    for row in m:
        for o in row:
            print(o, end='')
        print('')


if __name__ == '__main__':
    maze_w = input('Please input maze width (default: 10): ')
    maze_h = input('Please input maze height (default: 10): ')
    difficulty = input('Please input difficulty (default: 0.5): ')

    try:
        maze_w = int(maze_w)
    except:
        maze_w = 10
    try:
        maze_h = int(maze_h)
    except:
        maze_h = 10
    try:
        difficulty = float(difficulty)
    except:
        difficulty = 0.5

    maze = [[MARK_WALL] * maze_w for _ in range(maze_h)]

    gen_maze(maze, maze_w, maze_h, difficulty)

    if len(sys.argv) < 2:
        print(maze_to_str(maze, maze_w, maze_h))
    else:
        pass
        # with open(sys.argv[1], 'w+') as f:
        #     f.write(maze_to_str(maze, maze_w, maze_h))