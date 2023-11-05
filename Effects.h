#pragma once


struct Bubble
{
	Vec2 offset;
	double startTime;
	double scale;
	ColorF color;
};

struct BubbleEffect : IEffect
{
	Vec2 m_pos;

	Array<Bubble> m_bubbles;

	static constexpr double MaxDelayTime = 0.25;

	BubbleEffect(const Vec2& pos, double baseHue, bool delay = true)
		: m_pos{ pos }
	{
		for (int32 i = 0; i < 8; ++i)
		{
			Bubble bubble{
				.offset = RandomVec2(Circle{ 30 }),
				.startTime = (delay ? Random(-MaxDelayTime, 0.0) : 0.0), 
				.scale = Random(0.1, 1.2),
				.color = HSV{ baseHue + Random(-30.0, 30.0), 0.76, 1.0 }
			};
			m_bubbles << bubble;
		}
	}

	bool update(double t) override
	{
		constexpr double EffectDuration = 0.5;

		for (const auto& bubble : m_bubbles)
		{
			const double t2 = (bubble.startTime + t);

			if (not InRange(t2, 0.0, EffectDuration))
			{
				continue;
			}

			const double e = EaseOutExpo(t2 / EffectDuration);

			Circle{ (m_pos + bubble.offset), (e * 20 * bubble.scale) }
				.draw(ColorF{ bubble.color, 0.15 })
				.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
		}

		return (t < (EffectDuration + MaxDelayTime));
	}
};


struct Particle
{
	Vec2 start;

	Vec2 velocity;
};

struct Spark : IEffect
{
	Array<Particle> m_particles;

	explicit Spark(const Vec2& start)
		: m_particles(80)
	{
		for (auto& particle : m_particles)
		{
			particle.start = start + RandomVec2(Random(10.0));

			particle.velocity = Vec2::Down(20.0) + RandomVec2(1.0) * Random(Random(50.0));
		}
	}

	bool update(double t) override
	{
		for (const auto& particle : m_particles)
		{
			const Vec2 pos = particle.start
				+ particle.velocity * t + 0.5 * t * t * Vec2{ 0, 120 };

			Triangle{ pos, 4.0, (pos.x * 5_deg) }.draw(HSV{ 70, 0.1, 0.96, (1.0 - t) });
		}

		return (t < 1.0);
	}
};


struct Explosion : IEffect
{
	Array<Bubble> m_bubbles;
	double m_radius;
	Vec2 m_pos;

	Explosion(const Vec2& pos, double radius)
		: m_radius(radius - 20.0)
		, m_pos (pos)
	{
		for (int32 i = 0; i < 8; ++i)
		{
			Bubble bubble{
				.offset = RandomVec2(Circle{ m_radius / 2.0 }),
				.startTime = (Random(-0.3, 0.0)), // 登場の時間差
				.scale = Random(0.1, 1.2),
				.color = HSV{ 40 + Random(-20.0, 20.0) }
			};
			m_bubbles << bubble;
		}
	}

	bool update(double t) override
	{
		constexpr double EffectDuration = 0.5;

		const double a = EaseOutExpo(t / EffectDuration);
		Circle{ m_pos, m_radius }.draw(ColorF{1.0, 1-a}).drawFrame((30.0 * (1.0 - a) * 1), Palette::White);

		for (const auto& bubble : m_bubbles)
		{
			const double t2 = (bubble.startTime + t);

			if (not InRange(t2, 0.0, EffectDuration))
			{
				continue;
			}

			const double e = EaseOutExpo(t2 / EffectDuration);

			Circle{ (m_pos + bubble.offset), (e * m_radius * 0.5 * bubble.scale) }
				.draw(ColorF{ bubble.color, 0.15 })
				.drawFrame((30.0 * (1.0 - e) * bubble.scale), bubble.color);
		}

		return (t < (EffectDuration + 0.3));
	}
};


struct Wake : IEffect
{
	Array<Particle> m_particles;

	explicit Wake(const Vec2& start)
		: m_particles(4)
	{
		for (auto& particle : m_particles)
		{
			particle.start = start + RandomVec2(Random(2.0));

			particle.velocity = Vec2::Down(20.0) + RandomVec2() * Random(15.0);
		}
	}

	bool update(double t) override
	{
		for (const auto& particle : m_particles)
		{
			const Vec2 pos = particle.start
				+ particle.velocity * t + 0.5 * t * t * Vec2{ 0, 80 };

			Circle{ pos, 1.0 }.draw(HSV{ 190, 0.4, 0.96, (0.7 - t) });
		}

		return (t < 0.7);
	}
};

struct ColorParticle
{
	Vec2 start;
	Vec2 velocity;
	double hue;
};

struct RocketSmoke : IEffect
{
	Array<ColorParticle> m_particles;

	explicit RocketSmoke(const Vec2& start, const Vec2& dir)
		: m_particles(8)
	{
		for (auto& particle : m_particles)
		{
			particle.start = start + RandomVec2(Random(10.0));
			particle.velocity = dir.normalized() * 200 + Vec2{ 0, Random(-80.0, 80.0) };
			particle.hue = 16 + Random(-25.0, 25.0);
		}
	}

	bool update(double t) override
	{
		const double e = EaseOutExpo(t / 1.0);
		for (auto& particle : m_particles)
		{
			particle.velocity.y = e * particle.velocity.y;
			particle.velocity.x = (1.0 - t) * particle.velocity.x;
			const Vec2 pos = particle.start	+ particle.velocity * t;

			Circle{ pos, (1.0 - e) * 10.0 }.draw(HSV{ particle.hue, 0.8, (1.0 - t), (0.6 - t)});
		}
		return (t < 1.0);
	}
};

struct FireBallHit : IEffect
{
	Texture m_anim[4];
	double m_frameRate = 0.06;
	Vec2 m_pos;
	double m_rotation;
	explicit FireBallHit(const Vec2& pos, double rotation)
	{
		m_pos = pos;
		m_rotation = rotation + Random(-15_deg, 15_deg);
		m_anim[0] = Texture(Resource(U"resources/hit1.png"));
		m_anim[1] = Texture(Resource(U"resources/hit2.png"));
		m_anim[2] = Texture(Resource(U"resources/hit3.png"));
		m_anim[3] = Texture(Resource(U"resources/hit4.png"));
	}
	bool update(double t) override
	{
		int index = static_cast<int>(t / m_frameRate);
		if (index < 4)
		{
			m_anim[index].rotated(m_rotation).drawAt(m_pos);
		}
		return (index < 4);
	}
};

struct GoldTrace : IEffect
{
	Vec2 m_pos;
	double m_speed;
	explicit GoldTrace(const Vec2& pos, double speed)
	{
		m_pos = pos;
		m_speed = speed;
	}
	bool update(double t) override
	{
		RectF{ m_pos.x, m_pos.y, 10, 10 + Scene::DeltaTime() * Abs(m_speed)}.draw(HSV{42, 0.8, 1.0, (0.3 - t)});
		return (t < 0.3);
	}
};

struct Digging : IEffect
{
	Vec2 m_pos;
	Array<Bubble> m_bubbles;

	explicit Digging(const Vec2& pos)
		: m_pos(pos + Vec2::Down(12))
	{
		for (int32 i = 0; i < 8; ++i)
		{
			Bubble bubble{
				.offset = Vec2::Right(Random(-20, 20)),
				.startTime = -0.2,
				.scale = Random(0.3, 2.2),
				.color = HSV{ 35 + Random(-8.0, 8.0), 0.5, 0.75 }
			};
			m_bubbles << bubble;
		}
	}
	bool update(double t) override
	{
		constexpr double EffectDuration = 0.5;

		for (const auto& bubble : m_bubbles)
		{
			const double t2 = (bubble.startTime + t);

			if (not InRange(t2, 0.0, EffectDuration))
			{
				continue;
			}

			const double e = EaseOutQuad(t2 / EffectDuration);

			Circle{ (m_pos + bubble.offset - Vec2 { 0 , (t2 * 100)}), (e * 12 * bubble.scale) }
			.draw(ColorF{ bubble.color, 0.5 - t2 });
		}

		return (t < (EffectDuration - 0.2));
	}
};



struct Drain : IEffect
{
	Array<Bubble> m_bubbles;
	Vec2 m_pos;
	explicit Drain(const Vec2& pos)
		: m_pos(pos)
	{
		for (int32 i = 0; i < 4; ++i)
		{
			Bubble bubble{
				.offset = RandomVec2(Circle{ 75 }),
				.startTime =  Random(-0.2, 0.0),
				.scale = Random(0.2, 1.2),
				.color = HSV{ 248 + Random(-15.0, 15.0), 0.26, 0.94 }
			};
			m_bubbles << bubble;
		}
	}
	bool update(double t) override
	{
		constexpr double EffectDuration = 1.0;

		for (const auto& bubble : m_bubbles)
		{
			const double t2 = (bubble.startTime + t);

			if (not InRange(t2, 0.0, EffectDuration))
			{
				continue;
			}

			const double e = EaseOutExpo(t2 / EffectDuration);

			Circle{ (m_pos + bubble.offset * (1-e)), ( (1-e) * 10 * bubble.scale) }
				.draw(ColorF{ bubble.color, 0.25 })
				.drawFrame((10.0 * (e) * bubble.scale), ColorF(bubble.color, 0.15));
		}
		return (t < (EffectDuration - 0.2));
	}
};



struct Maho : IEffect
{
	struct particle {
		Vec2 pos;
		Vec2 velocity;
		int radius;
	};
	Array<particle> m_particles;

	explicit Maho(const Vec2& start)
		: m_particles(4)
	{
		m_particles[0] = {
			.pos = start + Vec2::Down(75 + Random(10.0)),
			.radius = 85
		};
		m_particles[1] = {
			.pos = start + Vec2::Down(65 + Random(10.0)),
			.radius = 70
		};
		m_particles[2] = {
			.pos = start + Vec2::Down(60 + Random(10.0)),
			.radius = 60
		};
		m_particles[3] = {
			.pos = start + Vec2::Down(58 + Random(10.0)),
			.radius = 50
		};
		for (auto& c : m_particles)
		{
			c.velocity = Vec2::Down(600.0 + Random(-50.0, 50.0));
		}
	}

	bool update(double t) override
	{

		for (auto& c : m_particles)
		{
			Vec2 pos = c.pos + t * c.velocity;
			Circle{ pos, c.radius }.draw(HSV{ 248, 0.07, 0.96, 0.1 });
		}
		return (t < 2.0);
	}
};
