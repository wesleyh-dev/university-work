import gym
env = gym.make('SpaceInvaders-v0')
for i_episode in range(5):
    observation = env.reset()
    while True:
        t=0
        env.render()  #comment out for faster training!
        #print(observation)
        action = env.action_space.sample()
        observation, reward, done, info = env.step(action)
        if done:
            print("Episode finished after {} timesteps".format(t+1))
            break
env.close()
