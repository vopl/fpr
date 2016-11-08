﻿
------------------------------------------------------------
DROP TABLE IF EXISTS track CASCADE;
CREATE TABLE track
(
	id bigserial PRIMARY KEY,
	name varchar NOT NULL UNIQUE
);
CREATE INDEX ON track(name);

------------------------------------------------------------
DROP TABLE IF EXISTS fingerprint CASCADE;
CREATE TABLE fingerprint
(
	id bigserial PRIMARY KEY,
	track_id bigint REFERENCES track(id) ON UPDATE CASCADE ON DELETE CASCADE,
	pos float4,
	val int8
);

CREATE INDEX ON fingerprint(track_id);
CREATE INDEX ON fingerprint(pos);
CREATE INDEX ON fingerprint(val);

------------------------------------------------------------
CREATE OR REPLACE FUNCTION query0(a bigint) RETURNS fingerprint AS $$
	SELECT * FROM fingerprint WHERE val = a
$$ LANGUAGE sql;

------------------------------------------------------------
CREATE OR REPLACE FUNCTION query1(a bigint) RETURNS fingerprint AS $$
	SELECT * FROM fingerprint WHERE val IN (
		a # x'0000000000000001'::int8,
		a # x'0000000000000002'::int8,
		a # x'0000000000000004'::int8,
		a # x'0000000000000008'::int8,
		
		a # x'0000000000000010'::int8,
		a # x'0000000000000020'::int8,
		a # x'0000000000000040'::int8,
		a # x'0000000000000080'::int8,
		
		a # x'0000000000000100'::int8,
		a # x'0000000000000200'::int8,
		a # x'0000000000000400'::int8,
		a # x'0000000000000800'::int8,
		
		a # x'0000000000001000'::int8,
		a # x'0000000000002000'::int8,
		a # x'0000000000004000'::int8,
		a # x'0000000000008000'::int8,
		
		a # x'0000000000010000'::int8,
		a # x'0000000000020000'::int8,
		a # x'0000000000040000'::int8,
		a # x'0000000000080000'::int8,
		
		a # x'0000000000100000'::int8,
		a # x'0000000000200000'::int8,
		a # x'0000000000400000'::int8,
		a # x'0000000000800000'::int8,
		
		a # x'0000000001000000'::int8,
		a # x'0000000002000000'::int8,
		a # x'0000000004000000'::int8,
		a # x'0000000008000000'::int8,
		
		a # x'0000000010000000'::int8,
		a # x'0000000020000000'::int8,
		a # x'0000000040000000'::int8,
		a # x'0000000080000000'::int8,
		
		a # x'0000000100000000'::int8,
		a # x'0000000200000000'::int8,
		a # x'0000000400000000'::int8,
		a # x'0000000800000000'::int8,
		
		a # x'0000001000000000'::int8,
		a # x'0000002000000000'::int8,
		a # x'0000004000000000'::int8,
		a # x'0000008000000000'::int8,
		
		a # x'0000010000000000'::int8,
		a # x'0000020000000000'::int8,
		a # x'0000040000000000'::int8,
		a # x'0000080000000000'::int8,
		
		a # x'0000100000000000'::int8,
		a # x'0000200000000000'::int8,
		a # x'0000400000000000'::int8,
		a # x'0000800000000000'::int8,
		
		a # x'0001000000000000'::int8,
		a # x'0002000000000000'::int8,
		a # x'0004000000000000'::int8,
		a # x'0008000000000000'::int8,
		
		a # x'0010000000000000'::int8,
		a # x'0020000000000000'::int8,
		a # x'0040000000000000'::int8,
		a # x'0080000000000000'::int8,
		
		a # x'0100000000000000'::int8,
		a # x'0200000000000000'::int8,
		a # x'0400000000000000'::int8,
		a # x'0800000000000000'::int8,
		
		a # x'1000000000000000'::int8,
		a # x'2000000000000000'::int8,
		a # x'4000000000000000'::int8,
		a # x'8000000000000000'::int8
		
		);
$$ LANGUAGE sql;