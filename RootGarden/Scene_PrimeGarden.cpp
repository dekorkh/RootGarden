#include "Scene_PrimeGarden.h"

Scene_PrimeGarden::Scene_PrimeGarden()
{
	const int Max_X = 50;
	const int Max_Y = 50;
	
	vector<vector<float>> Primes;
	
	for (int row_i = 0; row_i < Max_Y; row_i++)
	{
		vector<float> row;
		int begin = row_i * Max_X;
		int end = (row_i + 1) * Max_X;
		for (int elem_i = begin; elem_i < end; elem_i++)
		{
			if (IsPrime(elem_i))
			{
				float NormalizedValue = static_cast<float>(elem_i - begin) / Max_X;
				row.insert(row.end(), NormalizedValue);
			}
		}
		Primes.insert(Primes.end(), row);
	}

	for (int y = 0; y < Max_Y; y++)
	{
		for (int x = 0; x < Max_X; x++)
		{
			char c = ' ';
			for (auto& Prime : Primes[y])
			{
				float DenormPrime = Prime * Max_X;
				float diff = DenormPrime - x;
				if (diff >= 0 && diff < 1)
				{
					//c = 'x';// c = '\u23F9';
					MatterCircle* MatterCircleObject = new MatterCircle();
					MatterCircleObject->SetScale(Vector3f::Constant(0.01f));
					Vector3f NewPosition;
					NewPosition << (Prime * 2 - 1), ((static_cast<float>(y) / Max_Y) * 2 - 1), 0.0f;
					MatterCircleObject->SetPosition(NewPosition);
					int mem[50 * 50];
					float r = static_cast<float>(mem[y * Max_Y + x] % 1000) / 1000;
					float g = static_cast<float>(mem[y * Max_Y + x + 1] % 1000) / 1000;
					float b = static_cast<float>(mem[y * Max_Y + x + 2] % 1000) / 1000;
					Vector4f ColorInner;
					ColorInner << r, g, b, 1.0f;
					Vector4f ColorOuter;
					ColorOuter << 0.0f, 0.0f, 0.0f, 0.0f;

					MatterCircleObject->SetColors(ColorInner, ColorOuter);
					AddChild(MatterCircleObject);

					cout << mem[y * Max_Y + x] << ":" << mem[y * Max_Y + x + 1] << ":" << mem[y * Max_Y + x + 2];

				}
				else 
				{
					cout << c;
				}
			}
			
		}
		cout << endl;
	}
}


Scene_PrimeGarden::~Scene_PrimeGarden()
{
}

bool Scene_PrimeGarden::IsPrime(int n)
{
	bool Result = true;
	for (int i = 2; i < n; i++)
	{
		if (n % i == 0)
			Result = false;
	}
	return Result;
}