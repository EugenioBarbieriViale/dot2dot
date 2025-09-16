import numpy as np


class Env:
    def __init__(self, n):
        self.n = n
        self.erased = 0

        self.state_size = 36
        self.action_size = 3

        self.state = []
        self.reward = 0
        self.done = False

    def reset(self):
        state_list = []

        for i in range(self.n):
            row = []
            for j in range(self.n):
                if j < self.n - i:
                    row.append(0)
                else:
                    row.append(-1)

            state_list.append(row)

        self.reward = 0
        self.done = False
        self.state = np.array(state_list)
        self.erased = 0

        return self.state

    def act_random(self):
        y = np.random.randint(self.n-1)

        action = (y,
             np.random.randint(self.n - y),
             np.random.randint(self.n - y))

        return action

    def step(self, action, is_enemy=False):  # action = (y, x1, x2)
        direction = (1 if action[1] <= action[2] else -1)

        if self.erased == 21:
            self.done = True

            if not is_enemy:
                self.reward = 10

        if not self.done:
            for i in range(action[1], action[2] + direction, direction):
                if self.state[action[0]][i] != 0:
                    self.reward -= 50
                    break

                self.state[action[0]][i] = 1

                if not is_enemy:
                    self.reward -= 0.05

                self.erased += 1

        return self.state, self.reward, self.done
