import gym
import numpy as np

from QLearningAgents import DiscretizingQLearningAgent


EPISODES = 2000
WINDOW = 200
ConvergeThreshold=0.0001


if __name__ == "__main__":
    env = gym.make('CartPole-v0')
    # env.monitor.start('/tmp/CartPole-v0', force=True)

    mins = [-2.4, -3.0, -0.5, -2.0]
    maxs = [2.4, 3.0, 0.5, 2.0]
    bins = [np.linspace(max(mins[i], env.observation_space.low[i]),
                        min(maxs[i], env.observation_space.high[i]), num=10)
            for i in range(env.observation_space.shape[0])]

    agent = DiscretizingQLearningAgent(bins,
                                       env.action_space,
                                       learning_rate=0.05,
                                       discount=0.99,
                                       exploration_rate=0.5,
                                       exploration_decay_rate=0.99)

    total_rewards = []
    total_lengths = []
    for i_episode in range(EPISODES):
        observation = env.reset()
        agent.reset()
        total_reward = 0
        length = 0
        for j in range(200):
            # env.render()
            action = agent.act(observation)
            new_observation, reward, done, info = env.step(action)

            if done:
                reward = -100

            agent.update(observation, action, new_observation, reward)

            observation = new_observation

            total_reward += reward
            length += 1

            if done:
                break
        total_rewards.append(total_reward)
        total_lengths.append(length)
        print ("finished training episode: ", i_episode, " reward= ", total_reward)
        if(agent._maxChange<ConvergeThreshold):
            print ("Training converged. done training")
            break

    # env.monitor.close()
    # gym.upload('    CartPole-v0', api_key='API_KEY')
    print ("Done training after ", i_episode, " episodes")

    # plt.plot(np.convolve(total_lengths, np.ones((WINDOW,))/WINDOW, mode='valid'))
    # plt.xlabel("Episodes")
    # plt.ylabel("Total reward")
    # plt.show()

    for i in range(10):
        observation = env.reset()
        agent._exploration_rate = 0.0
        total_reward = 0
        for j in range(500):
            env.render()
            action = agent.act(observation)
            new_observation, reward, done, info = env.step(action)
            observation = new_observation
            total_reward += reward
            if done:
                break
        print(total_reward)