#pragma once
#include <fstream>
#include "Colour.hpp"
#include "Pixel.hpp"

class PixelColourMap
{
public:

	PixelColourMap()
		: m_Pixels(nullptr)
	{}

	~PixelColourMap()
	{
		delete[] m_Pixels;
	}

	static bool Init();

	static const PixelColourMap& Get();

	bool IsLoaded();

	void Build()
	{
		auto Convert = []( Colour a_Colour )
		{
			return 
				static_cast< int >( a_Colour.R ) +
				static_cast< int >( a_Colour.G ) * 256 +
				static_cast< int >( a_Colour.B ) * 65536;
		};

		// Set initial colours.
		for ( int i = 0; i < 16; ++i )
		{
			Pixel NewPixel;
			NewPixel.SetForegroundColour( ConsoleColours[ i ] );
			NewPixel.Unicode() = L'\x2588'; // Block
			m_Pixels[ Convert( SeedColours[ i ] ) ] = NewPixel;
		}

		size_t Index = 16;

		// Set remaining colours.
		for ( int i = 0; i < 16; ++i )
		{
			Colour Background = SeedColours[ i ];
			Pixel NewPixel;

			for ( int j = i + 1; j < 16; ++j )
			{
				Colour Foreground = SeedColours[ j ];

				for ( int k = 1; k < 4; ++k )
				{
					// Set alpha.
					Foreground.A = ( k - 1 ) * 64 + 63;
					
					// Create and set Colour Seed.
					Colour& SeedColour = SeedColours[ Index++ ];
					SeedColour = Background + Foreground;

					// Set Pixel.
					NewPixel.SetBackgroundColour( ConsoleColours[ i ] );
					NewPixel.SetForegroundColour( ConsoleColours[ j ] );
					NewPixel.Unicode() = L'\x2590' + k; // Dithering characters.
					m_Pixels[ Convert( SeedColour ) ] = NewPixel;
				}
			}
		}

		// Extrapolate to empty regions in cube.
		for ( int B = 0; B < 256; ++B )
		{
			for ( int G = 0; G < 256; ++G )
			{
				for ( int R = 0; R < 256; ++R )
				{
					int DiffVectorX;
					int DiffVectorY;
					int DiffVectorZ;
					int MinDistSqrd = 16777216;
					Pixel& Current = m_Pixels[ Convert( Colour( R, G, B ) ) ];
					Pixel Closest;

					for ( Colour Seed : SeedColours )
					{
						DiffVectorX = R - Seed.R;
						DiffVectorY = G - Seed.G;
						DiffVectorZ = B - Seed.B;
						int DistSqrd = DiffVectorX * DiffVectorX + DiffVectorY * DiffVectorY + DiffVectorZ * DiffVectorZ;

						if ( DistSqrd == 0 )
						{
							Closest = Current;
							break;
						}

						if ( DistSqrd < MinDistSqrd )
						{
							MinDistSqrd = DistSqrd;
							Closest = m_Pixels[ Convert( Seed ) ];
						}
					}

					Current = Closest;
				}
			}
		}
	}

	bool BuildAndSave()
	{
		Build();
		return Save();
	}

	bool Load()
	{
		std::fstream File;
		File.open( "../Resources/colours.map", std::ios::binary | std::ios::in );

		if ( !File.is_open() )
		{
			return false;
		}

		File.read( reinterpret_cast< char* >( m_Pixels ), 16777216 * sizeof( Pixel ) );
		File.close();
		return true;
	}

	bool Save()
	{
		std::fstream File;
		File.open( "../Resources/colours.map", std::ios::binary | std::ios::out );

		if ( !File.is_open() )
		{
			return false;
		}

		File.write( reinterpret_cast< char* >( m_Pixels ), 16777216 * sizeof( Pixel ) );
		File.close();
		return true;
	}

	Pixel ConvertColour( Colour a_Colour ) const
	{
		return m_Pixels[
			static_cast< int >( a_Colour.R ) +
			static_cast< int >( a_Colour.G ) * 256 +
			static_cast< int >( a_Colour.B ) * 65536 ];
	}

	static Colour SeedColours[376];

private:

	bool   m_IsLoaded = false;
	Pixel* m_Pixels = nullptr;
};