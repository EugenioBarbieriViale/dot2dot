import matplotlib.pyplot as plt
import numpy as np
import random

from collections import deque

import torch
import torch.nn as nn
import torch.nn.functional as F

from environment import Env
from hyperparameters import Hyperparameters


class DQN(nn.Module):
    def __init__(self, state_size, action_size):
        # super(DQN, self).__init__()
        super().__init__()

        action_components = [action_size, action_size, action_size]

        self.l1 = nn.Linear(state_size, 24)
        self.l2 = nn.Linear(24, 24)

        self.action_heads = nn.ModuleList([
                nn.Linear(24, num_actions) for num_actions in action_components
        ])

    def forward(self, x):
        x = F.relu(self.l1(x))
        x = F.relu(self.l2(x))

        return [head(x) for head in self.action_heads]


class Agent:
    def __init__(self, state_size, action_size, memory_len):
        self.state_size = state_size
        self.action_size = action_size

        self.memory = deque(maxlen=memory_len)

        self.model = DQN(state_size, action_size)
        self.optimizer = torch.optim.Adam(self.model.parameters(), lr=params.lr)

        self.eps = 0
        self.steps = 0

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def act(self, state):
        self.eps = params.eps_end + (params.eps_start - params.eps_end) * \
        np.exp(-1. * self.steps / params.eps_decay)

        self.steps += 1

        if np.random.rand() <= self.eps:
            return env.act_random()

        q_values_list = self.model(torch.FloatTensor(state))
        action = [torch.argmax(q_values).item() for q_values in q_values_list]
        return action

    def replay(self, batch_size):
        if len(self.memory) < batch_size:
            return

        minibatch = random.sample(self.memory, batch_size)

        for state, action, reward, next_state, done in minibatch:
            target = reward

            if not done:
                next_q_values = self.model(torch.FloatTensor(next_state))
                next_max_q = sum([torch.max(q_vals) for q_vals in next_q_values])
                target = reward + params.gamma * next_max_q

            # Get current Q-values
            current_q_values = self.model(torch.FloatTensor(state))

            # Calculate loss for each component
            total_loss = 0
            for i, (q_vals, action_component) in enumerate(zip(current_q_values, action)):
                target_q = q_vals.clone()
                target_q[action_component] = target
                loss = F.mse_loss(q_vals, target_q)
                total_loss += loss

            # Backpropagate
            self.optimizer.zero_grad()
            total_loss.backward()
            self.optimizer.step()


params = Hyperparameters()

n = 6
env = Env(n)

state_size = env.state_size
action_size = env.action_size

batch_size = 512

agent = Agent(state_size, action_size, 100)

for e in range(params.episodes):
    state = env.reset()
    state = np.reshape(state, [1, state_size])

    for time in range(30):
        action = agent.act(state)
        next_state, reward, done = env.step(action)

        print(env.erased)

        next_state = np.reshape(next_state, [1, state_size])

        agent.remember(state, action, reward, next_state, done)
        state = next_state

        if done:
            print(f"Episode {e+1}/{params.episodes} - Score: {time}, Epsilon: {agent.eps:.2f}")
            break

        if len(agent.memory) > batch_size:
            agent.replay(batch_size)

        state, _, _ = env.step(env.act_random(), is_enemy=True)
        state = np.reshape(state, [1, state_size])
