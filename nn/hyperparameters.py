class Hyperparameters:
    def __init__(self):
        self.episodes = 10

        self.gamma = 0.95
        self.lr = 3e-4

        self.eps_start = 0.99
        self.eps_end = 0.01
        self.eps_decay = 0.995
