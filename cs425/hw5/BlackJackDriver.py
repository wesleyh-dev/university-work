import gym
import numpy as np
import sys

from DeepRLAgent import DeepRLAgent

env = gym.make('Blackjack-v0')
#See details about the game rules here, but not necessary for your agent -- it will learn the rules by experimentation!
#Environment definition: https://github.com/openai/gym/blob/master/gym/envs/toy_text/blackjack.py
#actions, observations described in detail above
#so your policy network needs to learn to predict one of these actions based on the observation.

if '-train' in sys.argv[1:]:
    max_episodes_to_run = 2000
    max_timesteps_per_episode = 200

    goal_avg_episode_length = 195
    goal_consecutive_episodes = 100

    plot_episode_count = 200
    plot_redraw_frequency = 10

    agent = DeepRLAgent(env.observation_space, env.action_space)

    for i in range(max_episodes_to_run):
        observation = env.reset()
        action = agent.act(observation)
        for j in range(max_timesteps_per_episode):
            new_observation, reward, done, info = env.step(action)
            env.render()
            if done and j < max_timesteps_per_episode - 1:
                reward = -max_episodes_to_run
            agent.update(observation,action,new_observation,reward)
            action = agent.act(observation)
            if done or j == max_episodes_to_run - 1:
                break
    agent.save('training.txt')



if '-test' in sys.argv[1:]:
    agent = DeepRLAgent(env.observation_space, env.action_space)
    agent.load('training.txt')

    for i in range(100):
        observation = env.reset()
        total_rewards = 0
        for j in range(500):
            env.render()
            action = agent.act(observation)
            new_observation, reward, done, info = env.step(action)
            total_rewards += reward
            if done:
                break
        print(total_rewards)


# for i_episode in range(5):
#     total_rewards = 0
#     observation = env.reset()
#     while True:
#         t=0
#         #env.render()  #comment out for faster training!
#         print(observation)
#         action = env.action_space.sample() #random action, use your own action policy here
#
#         observation, reward, done, info = env.step(action)
#         total_rewards += reward
#         t += 1
#         if done:
#             print("Episode finished after {} timesteps %d with reward %d ", t, total_rewards)
#             break
# env.close()
